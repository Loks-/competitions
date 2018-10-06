#include "common/proxy_run_base.h"
#include "proxy_gen.h"

TSolutionsMap solutions_map {
	{ "armed_bandit", main_armed_bandit },
	{ "e", main_e },
	{ "jumping_over_walls", main_jumping_over_walls },
	{ "partitioning_a_square", main_partitioning_a_square },
	{ "qizz_quzz_q1", main_qizz_quzz_q1 },
	{ "qizz_quzz_q2", main_qizz_quzz_q2 },
};
