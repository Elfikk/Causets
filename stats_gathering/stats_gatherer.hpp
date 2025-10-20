#pragma once

#include "stats_gatherer.hpp"

#include "sprinkler/sprinkler.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class StatsGatherer
{
public:
    StatsGatherer(Sprinkler<d> & _sprinkler) : sprinkler(_sprinkler) {};

    void takeSamples(std::size_t minN, std::size_t maxN, std::size_t samples, std::size_t repeats);

    // Dump the data for Python to take care of.
    void dump(const std::string & orderingPath, const std::string & pathLengthPath) const;

private:
    void takeSample(std::size_t N);

    Sprinkler<d> sprinkler;

    std::map<std::size_t, std::vector<long double>> orderingFractionSamples = {};
    std::map<std::size_t, std::vector<long double>> pathLengthSamples = {};
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void StatsGatherer<d>::dump(const std::string & orderingPath, const std::string & pathLengthPath) const
{
    std::ofstream orderingDump(orderingPath);
    for (const auto & pair : orderingFractionSamples)
    {
        const auto N = pair.first;
        orderingDump << N;
        for (auto f : pair.second)
        {
            orderingDump << ",";
            orderingDump << f;
        }
        orderingDump << "\n";
    }
    orderingDump.close();

    std::ofstream pathDump(pathLengthPath);
    for (const auto & pair : pathLengthSamples)
    {
        const auto N = pair.first;
        pathDump << N;
        for (auto f : pair.second)
        {
            pathDump << ",";
            pathDump << f;
        }
        pathDump << "\n";
    }
    pathDump.close();
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void StatsGatherer<d>::takeSample(const std::size_t N)
{
    auto sprinkle = sprinkler.sprinkle(N);
    auto dag = sprinkle.generateCausalSet();

    long double relations = dag.getNumEdges();
    orderingFractionSamples[N].push_back(relations / (N * N));

    auto longestPath = dag.findLongestPath();
    pathLengthSamples[N].push_back((long double)longestPath.size());
}

//---------------------------------------------------------------------------------------------------------------------

template<int d>
void StatsGatherer<d>::takeSamples(
    const std::size_t minN,
    const std::size_t maxN,
    const std::size_t uniform_samples,
    const std::size_t repeats
)
{
    for (std::size_t i = 1; i < uniform_samples + 1; i++)
    {
        auto N = i * (maxN - minN) / uniform_samples + minN;
        std::cout << "Sampling at " << N << "\n";
        for (std::size_t repeat = 0; repeat < repeats; repeat++)
        {
            std::cout << "Taking sample " << repeat << " out of " << repeats <<"\n";
            takeSample(N);
        }
    }
}
