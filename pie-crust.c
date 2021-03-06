/*****************************************************
 *
 * copyright Michael D. Day 2017
 * made available under the GNU GPL, v2
 *
 ****************************************************/
#include "pie-crust.h"


static pid_t __pt_seize(pid_t pid) 
{
	int ccode = 0;
	
	ccode = ptrace(PTRACE_SEIZE, pid, NULL, NULL);
	if (ccode) {
		perror("ptrace seize ");
	}
	return ccode;
}


static pid_t __pt_wait(pid_t child) 
{
	int status;
	pid_t pid = waitpid(child, &status, WCONTINUED | WUNTRACED);
	if (pid == -1) {
		perror("waitpid");
		goto err_out;
	}

	if (WIFEXITED(status)) {
		printf("exited, status=%d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("killed by signal %d\n", WTERMSIG(status));
	} else if (WIFSTOPPED(status)) {
		printf("stopped by signal %d\n", WSTOPSIG(status));
	} else if (WIFCONTINUED(status)) {
		printf("continued\n");
	}

err_out:
	return pid;
}

int seize_and_wait(pid_t pid) 
{
	int ccode = __pt_seize(pid);
	if (ccode) {
		perror("seize_and_wait ");
		goto errout;
	}

	ccode = __pt_wait(pid);
	
errout:
		return ccode;	
}

/* [heap] */
uintptr_t map_after_heap(char *backing, int hole)
{

	procmaps_struct *iter = NULL;
	uintptr_t map_start = 0L;
	procmaps_struct *maps = pmparser_parse(getpid ());

	if (maps == NULL)
    {
		printf("unable to parse /proc/maps");
		return map_start;
    }

	while ((iter = pmparser_next ()) != NULL)
    {
		/* look for backing */
		if (iter->pathname)
		{
			char *pos = strstr (iter->pathname, backing);
			if (pos != NULL && strlen (pos) > 0)
			{
				/* grab the ending address, add a hole, 
				 * return the result */
				map_start = (uintptr_t) iter->addr_end + hole;
				goto out;
			}
		}
    }
out:
	if (maps != NULL)
    {
		pmparser_free (maps);
    }
	return map_start;
}




#define OPT_PID           0
#define OPT_ATTACH        1
#define OPT_STOP          2
#define OPT_MAP           3
#define OPT_HIJACK        4
#define OPT_VERBOSE       5
static struct option long_options[] = {
    {"pid", required_argument, 0, 0 },
	{"attach", no_argument, 0, 0 },
	{"stop", no_argument, 0, 0 },
	{"map", optional_argument, 0, 0 },
	{"hijack", required_argument, 0, 0 },
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
	printf("\t--hijack overwrite <function> of <pid> with my version\n");
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
	int ccode = get_and_check_options(argc, argv);
	pid_t tracee =  long_options[OPT_PID].val;
	ccode = seize_and_wait(tracee);

	return ccode;
}
