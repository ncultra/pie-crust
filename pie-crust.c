#include "pie-crust.h"


#define OPT_PID           0
#define OPT_ATTACH        1
#define OPT_STOP          2
#define OPT_MAP           3
#define OPT_VERBOSE       4
static struct option long_options[] = {
    {"pid", required_argument, 0, 0 },
	{"attach", no_argument, 0, 0 },
	{"stop", no_argument, 0, 0 },
	{"map", optional_argument, 0, 0 },
	{"verbose", no_argument, 0, 0 },
    {0, 0, 0, 0 }
};


static void usage(char *program)
{
	printf("%s: \n", program);
	printf("\t--pid <process id> pid of program to target\n");
	printf("\t--attach attach to the program <pid> \n");
	printf("\t--stop  stop the program <pid>\n");
	printf("\t--map map memory into the program <pid>\n");
	printf("\t--verbose display extra information when available\n");	
}



int get_and_check_options(int argc, char **argv)
{
	int option_index = 0, ccode = 0;

	while (1) {
		ccode = getopt_long_only(argc, argv,
								 "",
								 long_options,
								 &option_index);
		if (ccode == -1) {
			break;
		}

		switch (ccode) {
		case 0:
			switch (option_index) {
				char *endptr, *str;
				int val;
					
			case OPT_PID:
				errno = 0;
				str = optarg;
				val = strtol(optarg, &endptr, 10);
				if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
					|| (errno != 0 && val == 0)) {
					perror("strtol");
					exit(EXIT_FAILURE);
				}
				if (endptr == str) {
					fprintf(stderr, "strtol No digits were found\n");
					exit(EXIT_FAILURE);
				}
				long_options[option_index].val = val;
				break;
			case OPT_ATTACH:
			case OPT_STOP:
			case OPT_MAP:
			case OPT_VERBOSE:
				long_options[option_index].val = 1;
				break;	
			default:
				break;
			}

			break;
		default:
			usage(argv[0]);
			ccode = -1;
		}
	}
	return ccode;
}

int main(int argc, char **argv)
{
	int ccode;
	
	ccode = get_and_check_options(argc, argv);
	return ccode;
}
