//
//  atomic_constants.cpp
//  aces-ansatz
//
//  Created by Jason Byrd on 4/27/15.
//  Copyright (c) 2015 Jason Byrd. All rights reserved.
//

#include <stdio.h>
#include "molecule_data.h"
#include "input_reader.h"

namespace molecule {
    
    molecule::int_double return_atomic_constants(std::string atom_type) {
        
        molecule::int_double ID;
        
        int ret_charge;
        double ret_mass;
        
        if (atom_type.find("X") != std::string::npos )
        {
            ret_charge = 0;
            ret_mass = 0.0;
        }
        // Hydrogen
        else if (atom_type.find("H") != std::string::npos )
        {
            ret_charge = 1;
            ret_mass = 1.008;
        }
        // Helium
        else if (atom_type.find("He") != std::string::npos )
        {
            ret_charge = 2;
            ret_mass = 4.002602;
        }
        // Lithium
        else if (atom_type.find("Li") != std::string::npos )
        {
            ret_charge = 3;
            ret_mass = 6.94;
        }
        // Beryllium
        else if (atom_type.find("Be") != std::string::npos )
        {
            ret_charge = 4;
            ret_mass = 9.0121831;
        }
        // Boron
        else if (atom_type.find("B") != std::string::npos )
        {
            ret_charge = 5;
            ret_mass = 10.81;
        }
        // Carbon
        else if (atom_type.find("C") != std::string::npos )
        {
            ret_charge = 6;
            ret_mass = 12.011;
        }
        // Nitrogen
        else if (atom_type.find("N") != std::string::npos )
        {
            ret_charge = 7;
            ret_mass = 14.007;
        }
        // Oxygen
        else if (atom_type.find("O") != std::string::npos )
        {
            ret_charge = 8;
            ret_mass = 15.999;
        }
        // Fluorine
        else if (atom_type.find("F") != std::string::npos )
        {
            ret_charge = 9;
            ret_mass = 18.998403163;
        }
        // Neon
        else if (atom_type.find("Ne") != std::string::npos )
        {
            ret_charge = 10;
            ret_mass = 20.1797;
        }
        // Sodium
        else if (atom_type.find("Na") != std::string::npos )
        {
            ret_charge = 11;
            ret_mass = 22.98976928    ;
        }
        // Magnesium
        else if (atom_type.find("Mg") != std::string::npos )
        {
            ret_charge = 12;
            ret_mass = 24.305;
        }
        // Aluminium
        else if (atom_type.find("Al") != std::string::npos )
        {
            ret_charge = 13;
            ret_mass = 26.9815385;
        }
        // Silicon
        else if (atom_type.find("Si") != std::string::npos )
        {
            ret_charge = 14;
            ret_mass = 28.085;
        }
        // Phosphorus
        else if (atom_type.find("P") != std::string::npos )
        {
            ret_charge = 15;
            ret_mass = 30.973761998;
        }
        // Sulfur
        else if (atom_type.find("S") != std::string::npos )
        {
            ret_charge = 16;
            ret_mass = 32.06;
        }
        // Chlorine
        else if (atom_type.find("Cl") != std::string::npos )
        {
            ret_charge = 17;
            ret_mass = 35.45;
        }
        // Argon
        else if (atom_type.find("Ar") != std::string::npos )
        {
            ret_charge = 18;
            ret_mass = 39.948;
        }
        // Potassium
        else if (atom_type.find("K") != std::string::npos )
        {
            ret_charge = 19;
            ret_mass = 39.0983;
        }
        // Calcium
        else if (atom_type.find("Ca") != std::string::npos )
        {
            ret_charge = 20;
            ret_mass = 40.078;
        }
        // Scandium
        else if (atom_type.find("Sc") != std::string::npos )
        {
            ret_charge = 21;
            ret_mass = 44.955908;
        }
        // Titanium
        else if (atom_type.find("Ti") != std::string::npos )
        {
            ret_charge = 22;
            ret_mass = 47.867;
        }
        // Vanadium
        else if (atom_type.find("V") != std::string::npos )
        {
            ret_charge = 23;
            ret_mass = 50.9415;
        }
        // Chromium
        else if (atom_type.find("Cr") != std::string::npos )
        {
            ret_charge = 24;
            ret_mass = 51.9961;
        }
        // Manganese
        else if (atom_type.find("Mn") != std::string::npos )
        {
            ret_charge = 25;
            ret_mass = 54.938044;
        }
        // Iron
        else if (atom_type.find("Fe") != std::string::npos )
        {
            ret_charge = 26;
            ret_mass = 55.845;
        }
        // Cobalt
        else if (atom_type.find("Co") != std::string::npos )
        {
            ret_charge = 27;
            ret_mass = 58.933194;
        }
        // Nickel
        else if (atom_type.find("Ni") != std::string::npos )
        {
            ret_charge = 28;
            ret_mass = 58.6934;
        }
        // Copper
        else if (atom_type.find("Cu") != std::string::npos )
        {
            ret_charge = 29;
            ret_mass = 63.546;
        }
        // Zinc
        else if (atom_type.find("Zn") != std::string::npos )
        {
            ret_charge = 30;
            ret_mass = 65.38;
        }
        // Gallium
        else if (atom_type.find("Ga") != std::string::npos )
        {
            ret_charge = 31;
            ret_mass = 69.723;
        }
        // Germanium
        else if (atom_type.find("Ge") != std::string::npos )
        {
            ret_charge = 32;
            ret_mass = 72.630;
        }
        // Arsenic
        else if (atom_type.find("As") != std::string::npos )
        {
            ret_charge = 33;
            ret_mass = 74.921595;
        }
        // Selenium
        else if (atom_type.find("Se") != std::string::npos )
        {
            ret_charge = 34;
            ret_mass = 78.971;
        }
        // Bromine
        else if (atom_type.find("Br") != std::string::npos )
        {
            ret_charge = 35;
            ret_mass = 79.904;
        }
        // Krypton
        else if (atom_type.find("Kr") != std::string::npos )
        {
            ret_charge = 36;
            ret_mass = 83.798;
        }
        // Rubidium
        else if (atom_type.find("Rb") != std::string::npos )
        {
            ret_charge = 37;
            ret_mass = 85.4678;
        }
        // Strontium
        else if (atom_type.find("Sr") != std::string::npos )
        {
            ret_charge = 38;
            ret_mass = 87.62;
        }
        // Yttrium
        else if (atom_type.find("Y") != std::string::npos )
        {
            ret_charge = 39;
            ret_mass = 88.90584;
        }
        // Zirconium
        else if (atom_type.find("Zr") != std::string::npos )
        {
            ret_charge = 40;
            ret_mass = 91.224;
        }
        // Niobium
        else if (atom_type.find("Nb") != std::string::npos )
        {
            ret_charge = 41;
            ret_mass = 92.90637;
        }
        // Molybdenum
        else if (atom_type.find("Mo") != std::string::npos )
        {
            ret_charge = 42;
            ret_mass = 95.95;
        }
        // Technetium
        else if (atom_type.find("Tc") != std::string::npos )
        {
            ret_charge = 43;
            ret_mass = 96.906365;
        }
        // Ruthenium
        else if (atom_type.find("Ru") != std::string::npos )
        {
            ret_charge = 44;
            ret_mass = 101.07;
        }
        // Rhodium
        else if (atom_type.find("Rh") != std::string::npos )
        {
            ret_charge = 45;
            ret_mass = 102.90550;
        }
        // Palladium
        else if (atom_type.find("Pd") != std::string::npos )
        {
            ret_charge = 46;
            ret_mass = 106.42;
        }
        // Silver
        else if (atom_type.find("Ag") != std::string::npos )
        {
            ret_charge = 47;
            ret_mass = 107.8682;
        }
        // Cadmium
        else if (atom_type.find("Cd") != std::string::npos )
        {
            ret_charge = 48;
            ret_mass = 112.414;
        }
        // Indium
        else if (atom_type.find("In") != std::string::npos )
        {
            ret_charge = 49;
            ret_mass = 114.818;
        }
        // Tin
        else if (atom_type.find("Sn") != std::string::npos )
        {
            ret_charge = 50;
            ret_mass = 118.710;
        }
        // Antimony
        else if (atom_type.find("Sb") != std::string::npos )
        {
            ret_charge = 51;
            ret_mass = 121.760;
        }
        // Tellurium
        else if (atom_type.find("Te") != std::string::npos )
        {
            ret_charge = 52;
            ret_mass = 127.60;
        }
        // Iodine
        else if (atom_type.find("I") != std::string::npos )
        {
            ret_charge = 53;
            ret_mass = 126.90447;
        }
        // Xenon
        else if (atom_type.find("Xe") != std::string::npos )
        {
            ret_charge = 54;
            ret_mass = 131.293;
        }
        // Caesium
        else if (atom_type.find("Cs") != std::string::npos )
        {
            ret_charge = 55;
            ret_mass = 132.90545196;
        }
        // Barium
        else if (atom_type.find("Ba") != std::string::npos )
        {
            ret_charge = 56;
            ret_mass = 137.327;
        }
        // Lanthanum
        else if (atom_type.find("La") != std::string::npos ) 
        {
            ret_charge = 57;
            ret_mass = 138.90547;
        }
        // Cerium
        else if (atom_type.find("Ce") != std::string::npos ) 
        {
            ret_charge = 58;
            ret_mass = 140.116;
        }
        // Praseodymium    
        else if (atom_type.find("Pr") != std::string::npos ) 
        {
            ret_charge = 59;
            ret_mass = 140.90766;
        }
        // Neodymium
        else if (atom_type.find("Nd") != std::string::npos ) 
        {
            ret_charge = 60;
            ret_mass = 144.242;
        }
        // Samarium
        else if (atom_type.find("Sm") != std::string::npos ) 
        {
            ret_charge = 62;
            ret_mass = 150.36;
        }
        // Europium
        else if (atom_type.find("Eu") != std::string::npos ) 
        {
            ret_charge = 63;
            ret_mass = 151.964;
        }
        // Gadolinium
        else if (atom_type.find("Gd") != std::string::npos ) 
        {
            ret_charge = 64;
            ret_mass = 157.25;
        }
        // Terbium
        else if (atom_type.find("Tb") != std::string::npos ) 
        {
            ret_charge = 65;
            ret_mass = 158.92535;
        }
        // Dysprosium
        else if (atom_type.find("Dy") != std::string::npos ) 
        {
            ret_charge = 66;
            ret_mass = 162.500;
        }
        // Holmium
        else if (atom_type.find("Ho") != std::string::npos ) 
        {
            ret_charge = 67;
            ret_mass = 164.93033;
        }
        // Erbium
        else if (atom_type.find("Er") != std::string::npos ) 
        {
            ret_charge = 68;
            ret_mass = 167.259;
        }
        // Thulium
        else if (atom_type.find("Tm") != std::string::npos ) 
        {
            ret_charge = 69;
            ret_mass = 168.93422;
        }
        // Ytterbium
        else if (atom_type.find("Yb") != std::string::npos ) 
        {
            ret_charge = 70;
            ret_mass = 173.054;
        }
        // Lutetium
        else if (atom_type.find("Lu") != std::string::npos ) 
        {
            ret_charge = 71;
            ret_mass = 174.9668;
        }
        // Hafnium
        else if (atom_type.find("Hf") != std::string::npos ) 
        {
            ret_charge = 72;
            ret_mass = 178.49;
        }
        // Tantalum
        else if (atom_type.find("Ta") != std::string::npos ) 
        {
            ret_charge = 73;
            ret_mass = 180.94788;
        }
        // Tungsten
        else if (atom_type.find("W") != std::string::npos ) 
        {
            ret_charge = 74;
            ret_mass = 183.84;
        }
        // Rhenium
        else if (atom_type.find("Re") != std::string::npos ) 
        {
            ret_charge = 75;
            ret_mass = 186.207;
        }
        // Osmium
        else if (atom_type.find("Os") != std::string::npos ) 
        {
            ret_charge = 76;
            ret_mass = 190.23;
        }
        // Iridium
        else if (atom_type.find("Ir") != std::string::npos ) 
        {
            ret_charge = 77;
            ret_mass = 192.217;
        }
        // Platinum
        else if (atom_type.find("Pt") != std::string::npos ) 
        {
            ret_charge = 78;
            ret_mass = 195.084;
        }
        // Gold
        else if (atom_type.find("Au") != std::string::npos ) 
        {
            ret_charge = 79;
            ret_mass = 196.966569;
        }
        // Mercury
        else if (atom_type.find("Hg") != std::string::npos ) 
        {
            ret_charge = 80;
            ret_mass = 200.592;
        }
        // Thallium
        else if (atom_type.find("Tl") != std::string::npos ) 
        {
            ret_charge = 81;
            ret_mass = 204.38;
        }
        // Lead
        else if (atom_type.find("Pb") != std::string::npos ) 
        {
            ret_charge = 82;
            ret_mass = 207.2;
        }
        // Bismuth
        else if (atom_type.find("Bi") != std::string::npos ) 
        {
            ret_charge = 83;
            ret_mass = 208.98040;
        }
        // Protactinium
        else if (atom_type.find("Pa") != std::string::npos ) 
        {
            ret_charge = 91;
            ret_mass = 231.03588;
        }
        // Uranium
        else if (atom_type.find("U") != std::string::npos ) 
        {
            ret_charge = 92;
            ret_mass = 238.02891;
        }
        
        ID.charge = ret_charge;
        ID.mass = ret_mass;
        
        return ID;
    }
}
