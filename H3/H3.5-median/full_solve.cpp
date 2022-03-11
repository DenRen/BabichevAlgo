#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <queue>

#define NDEBUG

template <typename T = int>
class median_array {
    std::priority_queue <T> left_queue;
    std::priority_queue <T, std::vector <T>, std::greater <T>> right_queue;

public:
    median_array (T first_value) {
        left_queue.push (first_value);
    }

    T
    median () const {
        assert (left_queue.size () + right_queue.size ());

        return left_queue.size () >= right_queue.size () ?
                   left_queue.top () :
                   right_queue.top ();
    }

    void
    add (T value) {
        T med = median ();
        if (value <= med) {
            if (left_queue.size () > right_queue.size ()) {
                right_queue.push (left_queue.top ());
                left_queue.pop ();
            }
            left_queue.push (value);
        } else {
            if (right_queue.size () > left_queue.size ()) {
                left_queue.push (right_queue.top ());
                right_queue.pop ();
            }
            right_queue.push (value);
        }
    }

    void
    dump_arr () const {
        auto lq = left_queue, rq = right_queue;
        while (lq.size ()) {
            printf ("%d ", lq.top ()); lq.pop ();
        }
        printf ("\n");
        while (rq.size ()) {
            printf ("%d ", rq.top ()); rq.pop ();
        }
    }
};

void
exec_requests (unsigned num_req)
{
    int type = 0, first = 0;
    if (scanf ("%d %d", &type, &first) != 2 || type != 0) {
        throw std::invalid_argument ("Error read type or first value");
    }

    median_array <int> arr {first};
    --num_req;

    while (num_req--) {
        if (scanf ("%d", &type) != 1) {
            throw std::invalid_argument ("Error read type");
        }

        switch (type) {
        case 0: {
            int value = 0;
            if (scanf ("%d", &value) != 1) {
                throw std::invalid_argument ("Error read value");
            }
            arr.add (value);
        } break;

        case 1: {
            printf ("%d\n", arr.median ());
        } break;

        default:
            throw std::invalid_argument ("Incorrect request type");
            break;
        }
    }
}

int main () {
    unsigned num_req = 0;
    if (scanf ("%u\n", &num_req) != 1) {
        throw std::invalid_argument ("Failed read number reqs");
    }
    exec_requests (num_req);
}