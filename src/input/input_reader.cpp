//
//  input_reader.cpp
//  aces-ansatz
//
//  Created by Jason Byrd on 3/19/15.
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

namespace input{

    aces::aces_input_data reader(std::string input_string, std::string basis_string) {
        
        std::cout << std::endl << "Parsing input file" << std::endl;
        
        aces::aces_input_data InputData;
        
        // parse input file and store in ib struct
        input::input_block ib;
        ib.all_coordinate_vector.reserve(aces::GlobalSettings::get_starting_atom_count());
        ib = input::parse_file(input_string);
        
        // keywords first
        input::ioppar_flags f_flags;
        f_flags = input::keyword_handler(ib);
        
        
        // Now we start working with the molecule specification
        std::cout << std::endl << "----" << std::endl;
        std::cout << "Found " << molecule::GlobalMolecule::get_atom_count() << " atoms" << std::endl;
        
        std::stringstream ss;
        std::string isline;
        ss << ib.all_coordinate_vector[1];
        
        int ncol=0;
        while (!ss.eof()) {
            ++ncol;
            ss >> isline;
        }
        
        if (ncol < 4 && f_flags.ioppar[h_IOPPAR_coordinates-1] == 0 ) {
            std::cout << "Non-cartesian input coordinates not supported at this time" << std::endl;
            all_abort();
        } else if (f_flags.ioppar[h_IOPPAR_coordinates-1] == 3) {
            std::cout << "Using cartesian input." << std::endl;
            f_flags.ioppar[h_IOPPAR_coordinates-1] = 1;
        } else {
            std::cout << "Identified " << ncol << " columns in first coordinate entry.  Assuming cartesian input." << std::endl;
            f_flags.ioppar[h_IOPPAR_coordinates-1] = 1;
        }

        molecule::molecule_data MolD;
        MolD = input::store_cartesian_data(ib);
        
        /*
         If we are doing a fragment calculation, it is helpful to group the coordinates by fragment number.
         */
        
        if (f_flags.ioppar[h_IOPPAR_frag-1] == 0) {
            for (int i = 0; i < molecule::GlobalMolecule::get_atom_count(); ++i) {
                MolD.fragment_index[i] = 1;
            }
            molecule::GlobalMolecule::set_nfrag(0);
        } else {
            int *indices;
            indices = new int[molecule::GlobalMolecule::get_atom_count()];
            int *iswapped;
            iswapped = new int[molecule::GlobalMolecule::get_atom_count()];
            for (int i = 0; i < molecule::GlobalMolecule::get_atom_count(); ++i) { indices[i] = i; iswapped[i] = 0; }
            std::sort(indices, indices+molecule::GlobalMolecule::get_atom_count(),molecule::sort_indices(MolD.fragment_index));
            
            for (int i = 0; i < molecule::GlobalMolecule::get_atom_count(); ++i) {
                if (!iswapped[i] && !iswapped[indices[i]]) {
                    std::swap(MolD.atom_type[i], MolD.atom_type[indices[i]]);
                    for(int j=0;j<3;j++) { std::swap(MolD.xyz[i][j], MolD.xyz[indices[i]][j]); }
                    std::swap(MolD.fragment_index[i], MolD.fragment_index[indices[i]]);
                    std::swap(MolD.atom_basis_set_name[i], MolD.atom_basis_set_name[indices[i]]);
                    iswapped[i] = 1;
                    iswapped[indices[i]] = 1;
                }
            }
        }
        
        // print out updated coordinates
        std::cout << "Permuted and transformed coordinates:" << std::endl << std::endl;
        for (int i = 0; i < molecule::GlobalMolecule::get_atom_count(); ++i) {
            std::cout << MolD.atom_type[i] << " ";
            for(int j=0;j<3;j++) { std::cout.setf(std::ios_base::fixed); std::cout << std::setprecision(9) << MolD.xyz[i][j] << " "; }
            if (f_flags.ioppar[h_IOPPAR_frag-1]) {
                std::cout << MolD.fragment_index[i] << " ";
            }
            std::cout << std::endl;
        }
        
        InputData.MolD = MolD;
        InputData.f_flags = f_flags;
        
        return InputData;
        
    }


}