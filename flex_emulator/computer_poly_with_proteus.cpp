/*
	COMPUTER_POLY_WITH_PROTEUS.CPP
	------------------------------
*/
#include "computer_poly_with_proteus.h"

/*
	COMPUTER_POLY_WITH_PROTEUS::COMPUTER_POLY_WITH_PROTEUS()
	--------------------------------------------------------
*/
computer_poly_with_proteus::computer_poly_with_proteus()
	{
#ifdef NEVER
	/*
		Poly running as a terminal to CP/M
	*/
	ide::move_disk_to_user_space("PolyCPM.dsk");
	proteus.change_disk(0, ide::get_local_filename("PolyCPM.dsk").string().c_str());
#else
	/*
		Poly running POLYSYS
	*/
//	ide::move_disk_to_user_space("POLYSYS300_BASIC34.dsk");
//	proteus.change_disk(0, ide::get_local_filename("POLYSYS300_BASIC34.dsk").string().c_str());
	ide::move_disk_to_user_space("POLYSYS340_BASIC30.dsk");
	proteus.change_disk(0, ide::get_local_filename("POLYSYS340_BASIC30.dsk").string().c_str());
//	ide::move_disk_to_user_space("POLYSYS340_BASIC23.dsk");
//	proteus.change_disk(0, ide::get_local_filename("POLYSYS340_BASIC23.dsk").string().c_str());

	/*
		Load a blank FLEX disk into drive 1
	*/
	ide::move_disk_to_user_space("user.dsk");
	change_disk(1, ide::get_local_filename("user.dsk").string().c_str());
#endif

	/*
		Connect the Poly to the Proteus
	*/
	proteus.network.set_outstream(&network);
	network.set_outstream(&proteus.network);
	}

/*
	COMPUTER_POLY_WITH_PROTEUS::STEP()
	----------------------------------
*/
void computer_poly_with_proteus::step(uint64_t times)
	{
	proteus.cycles = cycles;
	for (uint64_t cycle = 0; cycle < times; cycle++)
		{
		computer_poly_1::step(1);
		proteus.step(1);
		}
	}

/*
	COMPUTER_POLY_WITH_PROTEUS::CHANGE_DISK()
	-----------------------------------------
*/
const char *computer_poly_with_proteus::change_disk(uint8_t drive, const char *filename)
	{
	return proteus.change_disk(drive, filename);
	}
