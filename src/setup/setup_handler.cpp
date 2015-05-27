//
//  setup_handler.cpp
//  aces-ansatz
//
//  Created by Jason Byrd on 4/27/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//

#include "molecule_data.h"
#include "input_reader.h"
#include "core.h"
#include "abort.h"
#include "flags.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fenv.h>
#include <ctime>
#include <execinfo.h>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include <iterator>     // back_inserter
#include <regex>        // regex, sregex_token_iterator
#include <locale>
#include <iomanip>

namespace aces {
    
    void Molecule_Setup(aces::aces_input_data InputData) {
        
        std::cout << "----" << std::endl;
        
// get atomic charge and mass
        molecule::int_double ID;
        for (int i = 0; i < molecule::GlobalMolecule::get_atom_count(); ++i) {
            std::string tmp_atom_type = InputData.MolD.atom_type[i];
            ID = molecule::return_atomic_constants(tmp_atom_type);
            InputData.MolD.charge[i] = ID.charge;
            InputData.MolD.mass[i] = ID.mass;
        }
        
    }

}
