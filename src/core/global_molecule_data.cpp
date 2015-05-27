//
//  molecule_data.cpp
//  aces-ansatz
//
//  Created by Jason Byrd on 4/1/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//

#include <stdio.h>
#include <string>

#include "molecule_data.h"

namespace molecule {
    
    int GlobalMolecule::atom_count = 0;
    
    std::string GlobalMolecule::all_atom_basis = "STO-3G";
    
    int GlobalMolecule::nfrag = 0;
    
}

