#include "solve.hpp"
#include "../../libs/other_func.hpp"
#include <fstream>

void
gen_test (std::string in_name, std::string out_name) {
    const int key_len_min = 1, key_len_max = 4096;
    const char symb_begin = 'a', symb_end = 'z';

    std::fstream is {in_name,  std::ios_base::out};
    std::fstream os {out_name, std::ios_base::out};

    seclib::RandomGenerator rand;

    auto gen_val = [&] () {
        const auto len = rand.get_rand_val <unsigned> (key_len_min, key_len_max);
        return rand.get_string (len, symb_begin, symb_end);
    };

    std::vector <std::string> saved_keys = {gen_val ()};    // Min one element
    auto gen_key = [&] () {
        if (rand.get_rand_val <unsigned> (10 + 1) >= 5 + 1) {
            saved_keys.push_back (gen_val ());
            return saved_keys[saved_keys.size () - 1];
        } else {
            const auto index = rand.get_rand_val <unsigned> (saved_keys.size ());
            return saved_keys[index];
        }
    };

    auto print_error  = [&] (bool is_not_error) {
        if (!is_not_error) {
            os << "ERROR\n";
        }
    };

    const std::size_t N_max = 1'000'000*0 + 300'000;
    db_native::data_base_t db_ref;

    is << N_max << "\n";

    int N = 0;
    for (; N < N_max / 2; ++N) {
        const auto key = gen_key (), val = gen_val ();

        is << "ADD " << key << " " << val << "\n";
        print_error (db_ref.insert (key, val));
    }

    // ref -> short form of reference
    std::string out_ref;
    for (; N < N_max; ++N) {
        const auto key = gen_key ();

        // insert, remove, update, find
        auto type = rand.get_rand_val <unsigned> () % 4;
        switch (type) {
            case 0: {   // Insert
                const auto val = gen_val ();
                is << "ADD " << key << " " << val << "\n";
                print_error (db_ref.insert (key, val));
            } break;
            case 1: {
                is << "DELETE " << key << "\n";
                print_error (db_ref.remove (key));
            } break;
            case 2: {
                const auto val = gen_val ();
                is << "UPDATE " << key << " " << val << "\n";
                print_error (db_ref.update (key, val));
            } break;
            case 3: {
                is << "PRINT " << key << "\n";
                
                bool state = db_ref.print (key, out_ref);
                print_error (state);
                if (state) {
                    os << key << " " << out_ref << "\n";
                }
            } break;
            default:
                throw std::runtime_error ("default!!!");
                break;
        }
    }
}

int main () {
    gen_test ("test.txt", "test.txt");
}