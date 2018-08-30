#include "proxy_gen.h"
#include <iostream>
#include <string>

int proxy(const std::string& s)
{
	if (false) return -1;
	else if (s == "bathroom_stalls") return main_bathroom_stalls();
	else if (s == "number_guessing") return main_number_guessing();
	else if (s == "senate_evacuation") return main_senate_evacuation();
	else if (s == "steed_2_cruise_control") return main_steed_2_cruise_control();
	else if (s == "cubic_ufo") return main_cubic_ufo();
	else if (s == "go_gopher") return main_go_gopher();
	else if (s == "saving_the_universe_again") return main_saving_the_universe_again();
	else if (s == "trouble_sort") return main_trouble_sort();
	else if (s == "bit_party") return main_bit_party();
	else if (s == "edgy_baking") return main_edgy_baking();
	else if (s == "waffle_choppers") return main_waffle_choppers();
	else if (s == "mysterious_road_signs") return main_mysterious_road_signs();
	else if (s == "mysterious_road_signs__slow") return main_mysterious_road_signs__slow();
	else if (s == "rounding_error") return main_rounding_error();
	else if (s == "transmutation") return main_transmutation();
	else if (s == "a_whole_new_word") return main_a_whole_new_word();
	else if (s == "ant_stack") return main_ant_stack();
	else if (s == "lollipop_shop") return main_lollipop_shop();
	else if (s == "field_trip") return main_field_trip();
	else if (s == "base") return main_base();
	std::cout << "Unknown solution name: " << s << std::endl;
	return -1;
}
