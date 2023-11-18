// Your goal is to do some searching/sorting in distributed data sets
// Easy: Find the greatest number -- start with this one.  A leader
// can do it with one broadcast and then pick the biggest number from
// everyone
//
// Hard: Pick the median
// This requires several rounds... You will need to add some commands
// and have the leader's reduce make several more broadcasts (or have
// the Distributed object pick new leaders and make new broadcasts)
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>

std::vector<int32_t>::iterator find_clossest(std::vector<int32_t>::iterator b, std::vector<int32_t>::iterator e, int32_t val) {
    auto lb = std::upper_bound(b, e, val);
    if (val <= (*(lb - 1) + *lb) / 2) {
        return lb - 1;
    }
    return lb;
}

// I've set this just to keep the numbers in a reasonable range when
// we print them out... for your run, please set it to 2 billion or so
constexpr int MAXELEMENT = INT32_MAX; // INT32_MAX;
class Process {
    // to find need to keep state over multiple iterations
    struct MedianState {
        std::vector<int32_t>::iterator l, r; // left pointer, right pointer, previous median
        int32_t prev_mg, offset;             // previous median argument
    } median_state;
    std::vector<int32_t> sorted_contents_;

public:
    // Add new commands here... Think of the process as a thing that receives
    // a broadcast command (one of these) and an argument and creates a response
    // One process is designated as the leader.  It will receive all responses
    // (including its own as the first element)
    typedef enum {
        PRINT = 0,
        VOTE_FOR_LEADER = 1,
        CHOOSE_MAX = 2,
        SIZE = 3,
        MEDIAN_PIVOT = 4,
        MEDIAN_SORT = 5,
        MEDIAN_BOUND = 6,
    } commands;

    // Just fill with some random data...
    Process(int32_t m) {
        for (int32_t i = 0; i < m; ++i) {
            sorted_contents_.push_back(std::rand() % MAXELEMENT);
        }
        std::sort(sorted_contents_.begin(), sorted_contents_.end());
        median_state = {.l = sorted_contents_.begin(), .r = sorted_contents_.end(), .prev_mg = -1, .offset = 0};
    }

    // This is cheating
    void cheat(std::vector<int32_t> &cheat_values) const {
        cheat_values.insert(cheat_values.end(),
                            sorted_contents_.begin(),
                            sorted_contents_.end());
    }

    // Only the leader will call broadcast... it will receive all the
    // responses and reduce it.
    int broadcast(std::vector<Process> &everyone, Process::commands c, int32_t argument) {
        std::vector<int32_t> responses;
        responses.push_back(command(c, argument));
        for (auto &p : everyone) {
            if (&p != this) {
                responses.push_back(p.command(c, argument));
            }
        }
        return reduce(c, argument, responses);
    }

    // Each process will execute this when it hears a broadcast
    int32_t command(Process::commands c, int32_t argument) {
        auto b = sorted_contents_.begin();
        auto e = sorted_contents_.end();
        switch (c) {
        case PRINT:
            for (const auto &x : sorted_contents_) {
                std::cout << x << ' ';
            }
            std::cout << std::endl;
            return 0;
        case VOTE_FOR_LEADER:
            // I will vote for being the leader by sending my biggest value
            // plus a random offset
            return sorted_contents_.back() + std::rand() % argument;
        case CHOOSE_MAX:
            return sorted_contents_.back();
        case SIZE:
            return sorted_contents_.size();
        case MEDIAN_SORT: {
            auto l = std::lower_bound(b, e, argument);
            auto u = std::upper_bound(b, e, argument);

            median_state.prev_mg = argument;
            return std::distance(l + std::distance(l, u) / 2, e);
        }
        case MEDIAN_BOUND: {
            median_state.offset = argument;
            if (median_state.l <= median_state.r) {
                if (argument > 0) {
                    median_state.r = std::upper_bound(median_state.l, e, median_state.prev_mg) - 1;
                } else if (argument < 0) {
                    median_state.l = std::upper_bound(b, median_state.r, median_state.prev_mg);
                } else if (argument == 0) {
                    median_state.r = find_clossest(median_state.l, median_state.r, median_state.prev_mg);
                    median_state.l = median_state.r;
                }
            }
            // std::cout << "*L: " << *median_state.l << ", *R: " << *median_state.r << ", *P: " << *median_state.p << ", " << argument << std::endl;
            return *(median_state.l + std::distance(median_state.l, median_state.r) / 2) == median_state.prev_mg;
        }
        case MEDIAN_PIVOT:
            if (median_state.r - median_state.l >= 0)
                return *(median_state.l + std::distance(median_state.l, median_state.r) / 2);
            break;
        }
        return -1;
    }

    // The leader can see all the responses
    int32_t reduce(Process::commands c, int32_t argument, const std::vector<int32_t> &responses) {
        auto b = responses.begin();
        auto e = responses.end();
        // std::cout << "\nreduce: ";
        // for (const auto &e : responses) {
        // std::cout << e << ' ';
        // }
        // std::cout << std::endl;
        switch (c) {
        case PRINT:
            return -1;
        case VOTE_FOR_LEADER:
        case CHOOSE_MAX:
            return *std::max_element(b, e);
        case MEDIAN_PIVOT: {
            auto tmp = -1;
            while (tmp == -1)
                tmp = responses[rand() % responses.size()];
            return tmp;
        }
        case MEDIAN_SORT:
        case SIZE:
        case MEDIAN_BOUND:
            return std::accumulate(b, e, 0);
        default:
            break;
        }
        return -1;
    }
};

class Distributed {
public:
    Distributed(int M, int from, int to) {
        for (int i = 0; i < M; ++i) {
            int m = from + std::rand() % (to - from + 1);
            processes.emplace_back(m);
        }
    }

    void print_all() {
        auto *leader = pick_a_leader();
        leader->broadcast(processes, Process::PRINT, 0);
    }

    int max() {
        auto *leader = pick_a_leader();
        return leader->broadcast(processes, Process::CHOOSE_MAX, 0);
    }

    int median() {
        auto *leader = pick_a_leader();
        int32_t half_size = (leader->broadcast(processes, Process::SIZE, 0) + 1) / 2;
        // std::cout << "Half size:\t" << half_size << std::endl;
        int32_t m = -1, bigger, i = 0;
        auto offset = 0;
        do {
            m = leader->broadcast(processes, Process::MEDIAN_PIVOT, m); // use mean median as first guess
            // std::cout << "guess median: " << m << std::endl;
            bigger = leader->broadcast(processes, Process::MEDIAN_SORT, m);
            // std::cout << "bigger: " << bigger << std::endl;
            offset = leader->broadcast(processes, Process::MEDIAN_BOUND, half_size - bigger);
            // std::cout << "offset: " << offset << std::endl;
        } while ((i++ < 1000) && ((half_size - bigger) != 0));
        // std::cout << "i:\t" << i << std::endl;
        return m;
    }

    std::vector<int32_t> cheat() const {
        std::vector<int32_t> all_values;
        for (auto &p : processes) {
            p.cheat(all_values);
        }
        std::sort(all_values.begin(), all_values.end());
        return all_values;
    }

private:
    std::vector<Process> processes;
    Process *pick_a_leader() {
        if (processes.size() == 0)
            return nullptr;

        // everyone votes, pick the biggest vote
        Process *leader = &processes.front();
        int maxvalue = leader->command(Process::VOTE_FOR_LEADER, processes.size());
        for (auto &p : processes) {
            int vote = p.command(Process::VOTE_FOR_LEADER, processes.size());
            if (vote > maxvalue) {
                leader = &p;
                maxvalue = vote;
            }
        }
        return leader;
    }
};

int main() {
    // Providing a seed value.  For debug, just pick a constant integer here
    auto seed = (unsigned)time(NULL);
    // auto seed = (unsigned)1700293873;
    std::srand(seed);

    std::cout << "seed:\t" << seed << std::endl;

    Distributed D(100 /* processes */, 1000, 1500 /* with between 10 and 15 numbers */);
    auto cheat_arr = D.cheat();

    std::cout << "dist max:\t" << D.max() << std::endl;
    std::cout << "cheat max:\t" << cheat_arr.back() << std::endl
              << std::endl;

    // D.print_all();

    // std::cout << "total len:\t" << cheat_arr.size() << std::endl;
    std::cout << "cheat true median:\t" << (cheat_arr[(cheat_arr.size()) / 2] + cheat_arr[(cheat_arr.size() - 1) / 2]) / 2 << std::endl;
    std::cout << "cheat one of median:\t" << cheat_arr[(cheat_arr.size()) / 2] << ", " << cheat_arr[(cheat_arr.size() - 1) / 2] << std::endl;
    std::cout << "distrib median:\t" << D.median() << std::endl;
    //some times =)


    // int i = 0;
    // for (const auto &x : D.cheat()) {
    //     std::cout << i++ << ":" << x << ' ';
    // }
    // std::cout << std::endl;
    return 0;
}