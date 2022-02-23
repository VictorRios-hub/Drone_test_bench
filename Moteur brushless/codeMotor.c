/**
*
*
* @author     Vico
* @date       now
*
*
*/
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h> // for atoi
#include <signal.h>
#include <rc/time.h>
#include <rc/dsm.h>
#include <rc/servo.h>
#include <rc/encoder.h>

static int running = 0;

typedef enum test_mode_t {
	DISABLED,
	NORM,
	WIDTH,
	SWEEP,
	RADIO
}test_mode_t;
static void __print_usage(void)
{
	printf("\n");
	printf(" Notre fonctionnement\n");
	printf(" -c {channel}   Specify one channel to be driven from 1-8.\n");
	printf("                Otherwise all channels will be driven equally\n");
	printf(" -f {hz}        Specify pulse frequency, otherwise 50hz is used\n");
	printf(" -t {throttle}  Throttle to send between -0.1 & 1.0\n");
	printf(" -s {max}       Gently sweep throttle from 0 to {max} back to 0 again\n");
	printf("                {max} can be between 0 & 1.0\n");
	printf("\n");
	printf("sample use to inititialize and start brushless motor :  sudo ./test -s 0.5 \n");
	printf("\n");
}
// interrupt handler to catch ctrl-c
static void __signal_handler(__attribute__((unused)) int dummy)
{
	running = 0;
	return;
}
int main(int argc, char *argv[])
{
	int c, i, j;            // misc variables
	double thr = 0;         // normalized throttle
	double sweep_limit = 0; // max throttle allowed when sweeping
	int ch = 0;             // channel to test, 0 means all channels
	double dir = 1;         // switches between 1 & -1 in sweep mode
	test_mode_t mode;       // current operating mode
	int frequency_hz = 50;  // default 50hz frequency to send pulses
	int wakeup_en = 1;      // wakeup period enabled by default
	double wakeup_s = 3.0;  // wakeup period in seconds
	double wakeup_val = -0.1;// wakeup value
	int min_us = RC_ESC_DEFAULT_MIN_US;
	int max_us = RC_ESC_DEFAULT_MAX_US;
	// start with mode as disabled
	mode = DISABLED;
	// parse arguments

	while ((c = getopt(argc, argv, "c:f:t:ow:s:r:hdp:m:")) != -1) {
		switch (c) {

			// channel option
		case 'c':
			ch = atoi(optarg);
			if (ch<RC_SERVO_CH_MIN || ch>RC_SERVO_CH_MAX) {
				fprintf(stderr, "ERROR channel option must be between %d and %d\n", RC_SERVO_CH_MIN, RC_SERVO_CH_MAX);
				return -1;
			}
			break;

			// pulse frequency option
		case 'f':
			frequency_hz = atoi(optarg);
			if (frequency_hz<1) {
				fprintf(stderr, "Frequency option must be >=1\n");
				return -1;
			}
			break;

			// throttle
		case 't':
			// make sure only one mode in requested
			if (mode != DISABLED) {
				fprintf(stderr, "ERROR please select only one mode to use\n");
				__print_usage();
				return -1;
			}
			thr = atof(optarg);
			if (thr>1.0 || thr<-0.1) {
				fprintf(stderr, "ERROR throttle must be from -0.1 to 1\n");
				return -1;
			}
			mode = NORM;
			break;

			// sweep mode option
		case 's':
			// make sure only one mode in requested
			if (mode != DISABLED) {
				fprintf(stderr, "ERROR please select only one mode to use\n");
				__print_usage();
				return -1;
			}
			sweep_limit = atof(optarg);
			if (sweep_limit>1.0 || sweep_limit<0) {
				fprintf(stderr, "ERROR: Sweep limit must be from 0 to 1.0\n");
				return -1;
			}
			mode = SWEEP;
			thr = 0;
			dir = -1;
			break;
		}
	}
	// if the user didn't give enough arguments, exit
	if (mode == DISABLED) {
		fprintf(stderr, "\nNot enough input arguments\n");
		__print_usage();
		return -1;
	}
	// set signal handler so the loop can exit cleanly
	signal(SIGINT, __signal_handler);
	running = 1;
	// initialize PRU and make sure power rail is OFF
	if (rc_servo_init()) return -1;
	if (rc_servo_set_esc_range(min_us, max_us)) return -1;
	rc_servo_power_rail_en(0);

	// if driving an ESC, send throttle of 0 first
	// otherwise it will go into calibration mode
	if (wakeup_en) {
		printf("waking ESC up from idle for 3 seconds\n");
		for (i = 0; i <= frequency_hz*wakeup_s; i++) {
			if (running == 0) return 0;
			if (rc_servo_send_esc_pulse_normalized(ch, wakeup_val) == -1) return -1;
			rc_usleep(1000000 / frequency_hz);
		}
		printf("done with wakeup period\n");
	}

	for (j = 0; j < 200; j++)
	{
		rc_servo_send_pulse_us(0, 1500);
		rc_usleep(20000);
	}
	printf("Initialisation done \n");

	// initialize hardware first
	if(rc_encoder_init()){
			fprintf(stderr,"ERROR: failed to run rc_encoder_init\n");
			return -1;
	}

	// set signal handler so the loop can exit cleanly
	signal(SIGINT, __signal_handler);
	printf("\nRaw encoder positions\n");
	printf("      E1   |");
	printf(" \n");
	
	// Main loop runs at frequency_hz
	while (running) 
	{
		switch (mode) {
			
		case SWEEP:
			// increase or decrease position each loop
			// scale with frequency
			thr += dir * sweep_limit / frequency_hz;
			// reset pulse width at end of sweep
			if (thr > sweep_limit) {
				thr = sweep_limit;
				dir = -1;
			}
			else if (thr < 0) {
				thr = 0;
				dir = -1;
			}
			// send result
			rc_servo_send_pulse_us(0, 1600);
			printf("\r");
			printf("%10d |", rc_encoder_read(1));
			fflush(stdout);
			break;
			default: fprintf(stderr, "ERROR unhandled mode\n");
			return -1;
		}
		// sleep roughly enough to maintain frequency_hz
		rc_usleep(1000000 / frequency_hz);
	}
	// cleanup
	rc_encoder_cleanup();
	rc_servo_send_esc_pulse_normalized(ch, -0.1);
	rc_usleep(50000);
	rc_servo_cleanup();
	rc_dsm_cleanup();
	printf("\n");
	return 0;
}
