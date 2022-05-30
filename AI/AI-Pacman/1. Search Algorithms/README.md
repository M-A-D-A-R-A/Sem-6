[Instructions Doc](/1.%20Search%20Algorithms/search_algorithms_documentation.pdf)

* Depth First Search
* Breadth First Search
* Uniform Cost Search
* A* Search
* Corners Problem: Representation
* Corners Problem: Heuristic
* Eating All Dots: Heuristic
* Suboptimal Search

The main ideas of implementation were to analyze and build general search algorithms and apply them to Pacman scenarios to make the search problem run efficiently. I added the search algorithms in search.py by using the algorithm prompts and pseudo code as reference and building on it. It was really helpful and made the process really smooth. The implemented algorithms are as follows : Depth First Search (dfs), Breadth First Search (bfs), Uniform Cost Search (ucs) and A Star Search (star). In addition to that solved the given problems using the most effective heuristic to follow admissibility and consistency. Creating the non trivial heuristic was a little bit tricky but eventually made it through. cornersHeuristic and foodHeuristic was mainly to take the shortest distance in consideration such as Manhattan and mazeDistance respectively. Also used aStarSearch as the search option for finding PathToClosestDot.
