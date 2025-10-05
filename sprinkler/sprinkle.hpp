#pragma once

#include "graphs/directed_graph.hpp"

#include <algorithm>
#include <vector>

template<int d>
class Sprinkle
{
public:
    Sprinkle() {}
    void addEvent(Event<d> & event) { events.emplace_back(event); }

    // Do I want the entire class templated on the Spacetime?
    DirectedGraph generateCausalSet(Spacetime<d> * spacetime);

    std::vector<Event<d>> getEvents() { return events; }

private:
    std::vector<Event<d>> events;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
DirectedGraph Sprinkle<d>::generateCausalSet(Spacetime<d> * spacetime)
{
    // Sort the events by time within the vector
    std::sort(
        events.begin(),
        events.end(),
        [](Event<d> a, Event<d> b) { return a.time() < b.time(); }
    );

    DirectedGraph graph = DirectedGraph(events.size());

    for (std::size_t i = 0; i < events.size() - 1; i++)
    {
        for (std::size_t j = i + 1; j < events.size(); j++)
        {
            auto relation = spacetime->causalRelation(events[i], events[j]);
            if (relation == CausalRelation::CausalFuture)
            {
                graph.addEdge(i, j);
            }
        }
    }

    return graph;
}
