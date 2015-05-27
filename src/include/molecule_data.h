//
//  molecule_data.h
//  aces-ansatz
//
//  Created by Jason Byrd on 3/19/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//

#ifndef aces_ansatz_molecule_data_h
#define aces_ansatz_molecule_data_h

#include <string>
#include "input_reader.h"

namespace molecule {
    
    class GlobalMolecule {
        public:
        
        static int get_atom_count() { return atom_count; }
        static void set_atom_count(int s) { atom_count = s; }
        
        static std::string get_atom_basis() { return all_atom_basis; }
        static void set_atom_basis(std::string ss) { all_atom_basis = ss; }
        
        static int get_nfrag() { return nfrag; }
        static void set_nfrag(int s) {nfrag = s; }
        
        private:
        static int atom_count;
        static std::string all_atom_basis;
        static int nfrag;
        
    };
    
    class sort_indices
    {
    private:
        int* mparr;
    public:
        sort_indices(int* parr) : mparr(parr) {}
        const bool operator()(int i, int j) { return mparr[i]<mparr[j]; }
    };
    
    
    struct int_double {
        int charge;
        double mass;
    };
    
    molecule::int_double return_atomic_constants(std::string);
}

#endif
