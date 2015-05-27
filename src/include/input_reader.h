//
//  input_reader.h
//  aces-ansatz
//
//  Created by Jason Byrd on 3/19/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//


#include "core.h"
#include "molecule_data.h"
#include "flags.h"
#include <string>
#include <vector>

#ifndef __aces_ansatz__input_reader__
#define __aces_ansatz__input_reader__

#define max_asv 260
#define max_ioppar 600

#define h_ICHAR_handle		0
#define h_ICHAR_string		1
#define h_ICHAR_long		2
#define h_ICHAR_double		3
#define h_ICHAR_long_array	4
#define h_ICHAR_double_array	5

#define bVerbose_Update 1

namespace molecule {
    
    struct molecule_data {
        std::string* atom_type;
        double** xyz;
        int* fragment_index;
        std::string* atom_basis_set_name;
        int* charge;
        double* mass;
    };
}

namespace input {
    
    struct input_block {
        std::vector<std::string> all_coordinate_vector;
        std::vector<std::string> all_keyword_string_vector;
    };
    
    input_block parse_file(std::string);
    
    void count_atom(std::string);
    
    std::string comment_removed_line(std::string);

    void dump_file_string(int, std::string);
    
    molecule::molecule_data store_cartesian_data(input::input_block);
    
    struct ASV_nl_t {
        std::string alias;  /* a plain string for whole key token matching */
        std::string oppar;  /* a key definition with a stub delimiter */
        int ichar;  /* a value type */
        int ideflt; /* the default value */
        std::string units;  /* value units (for printing) */
    };
    
    struct ioppar_flags {
        int* ioppar;
    };
    
    class GlobalKeywords {
    public:
        static int get_max_asv() { return max_asv; }
        static int get_max_ioppar() { return max_ioppar; }
    };
    
    input::ioppar_flags keyword_handler(input::input_block);
    
    input::ioppar_flags init_keyword_flags();
    input::ioppar_flags parse_keyword_flags(input::input_block);
    void asv_dump(input::ioppar_flags);
    int asv_handle_proc(int, std::string);
    int asv_update_handle(int, std::string, const char*);

}

namespace aces {
    
    struct aces_input_data {
        molecule::molecule_data MolD;
        input::ioppar_flags f_flags;
    };
    
}

namespace input {
    
    aces::aces_input_data reader(std::string, std::string);
    
}

extern input::ASV_nl_t ASV_nl[max_asv];

#endif /* defined(__aces_ansatz__input_reader__) */
