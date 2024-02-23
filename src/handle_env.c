#include "../include/malloc.h"


/* @brief check env variable to know is we need to build debug data */
static char *check_env_variable(char *to_check)
{
	char *env = getenv(to_check);
	if (env) {
		// ft_printf_fd(1, PURPLE"%s enable: %s\n"RESET, to_check, env);
		return (env);
	}
	return (NULL);
}

/* @brief basic check flag function return bool value */
inline int16_t check_debug_flag(int16_t flag) { return ((g_data->type & flag) == flag); }

/* @brief get debug file fd */
inline int get_debug_fd() { return (g_data->fd); }

void display_env_var(char *env, char *to_check)
{
	char *str = "Enable";
	char *color = GREEN;
	if (!env) {
		str = "Disable";
		color = RED;
	}
	ft_printf_fd(1, "%s%s %s: %s\n"RESET, color, to_check, str, env);
}

static int16_t bool_check_env(char *to_check, int16_t flag, int16_t display)
{
	char *env = getenv(to_check);

	if (display != 0) {
		display_env_var(env, to_check);
	}
	/*if env == NULL --> 0 * FLAG, otherwise --> 1 *FLAG*/
	return ((env != NULL) * flag);
}

int handle_env_variable(int16_t *special_flag)
{
	int 	fd = -1;
	char	*trace_file = NULL;
	int16_t	display = bool_check_env(MALLOC_DISPLAY_ENV, ENV_DISPLAY\
		, bool_check_env(MALLOC_DISPLAY_ENV, ENV_DISPLAY, 0));
	
	trace_file = check_env_variable(MALLOC_TRACE_ENV);
	if (trace_file) {
		fd = open(trace_file, O_CREAT | O_APPEND | O_WRONLY, 00666);
		if (fd > 0) {
			*special_flag += ALLOCATION_TRACE;
			ft_printf_fd(1, "%s%s Enable: %s\n"RESET, GREEN, MALLOC_TRACE_ENV, trace_file);

		} else {
			ft_printf_fd(2, RED"Error can't open or create file %s\n"RESET, trace_file);
			fd = -1;
		}
	}
	*special_flag += bool_check_env(MALLOC_COLOR_ENV, ENABLE_COLOR, display);
	*special_flag += bool_check_env(MALLOC_LEAKS_ENV, DETECT_LEAK, display);
	*special_flag += bool_check_env(MALLOC_GARBAGE_ENV, GARBAGE_COLLECTOR_FREE, display);
	return (fd);
}
