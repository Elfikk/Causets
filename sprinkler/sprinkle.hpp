#pragma once

#include "graphs/directed_graph.hpp"

#include <algorithm>
#include <functional>
#include <optional>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------

template<int d>
class Sprinkle
{
public:
    Sprinkle(std::function<CausalRelation(const Event<d> &, const Event<d> &)> func) : causalRelationFunc(func) {}
    void addEvent(Event<d> & event) { events.emplace_back(event); }

    // Do I want the entire class templated on the Spacetime?
    // Different types of sprinkles could benefit from optimisations i.e above
    // a certain time, all points must be related in a rectangular region.
    DirectedGraph generateCausalSet();

    std::vector<Event<d>> getEvents() { return events; }

private:
    std::vector<Event<d>> events;
    std::function<CausalRelation(const Event<d> &, const Event<d> &)> causalRelationFunc;
};

//---------------------------------------------------------------------------------------------------------------------

template<int d>
DirectedGraph Sprinkle<d>::generateCausalSet()
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
            auto relation = causalRelationFunc(events[i], events[j]);
            if (relation == CausalRelation::CausalFuture)
            {
                graph.addEdge(i, j);
            }
        }
    }
    return graph;
}
