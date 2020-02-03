#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct propagationResult {
    int32_t depth;
    int32_t farthestPoint;
};

/****************************************************************
   Function propagate(...) is called recursively,
   returns maximum depth
   and the farthest point from the current point
*****************************************************************/
propagationResult propagate(int32_t currentPoint,
                            int32_t previousPoint, //where we come here from, don't go back
                            int32_t parents[],     //the first column in the input file
                            int32_t children[],    //the second column in the input file
                            int32_t relaysNumber) {
    int32_t depth = 1;
    int32_t farthestPoint = currentPoint;

    int32_t neighbor;
    for (int32_t i = 0; i < relaysNumber; i++) {
        if (parents[i] != currentPoint && children[i] != currentPoint) continue;
        if (parents[i] == previousPoint || children[i] == previousPoint) continue;
        if (parents[i] == currentPoint) {
            neighbor = children[i];
        } else { // if (children[i] == currentPoint)
            neighbor = parents[i];
        }
        propagationResult neighborResult = propagate(neighbor,     //recursion for each neighbor
                                                     currentPoint, //"previous" for the neighbor
                                                     parents,
                                                     children,
                                                     relaysNumber);
        int32_t childDepth = neighborResult.depth;
        if (childDepth >= depth) {
            depth = childDepth + 1;
            farthestPoint = neighborResult.farthestPoint;
        }
    }
    propagationResult result = {depth, farthestPoint};
    return result;
}

int main (int argc, char* argv[]) {
    /************************************************************
     Reading data from file
    ************************************************************/
    if (argc < 2) {
        cout << "No path argument. Example: virality2.exe dataset.txt";
		return 1;
    }
    cout << argv[0] << " " << argv[1] << "\n";
	char *fileName = argv[1];

	ifstream file(fileName);
	if (!file.is_open()) {
		cout << "Can't open file " << fileName;
		return 2;
	}
    string line;
    getline (file, line);
    stringstream firstLineStream = stringstream(line);
    int32_t nRelays;
    firstLineStream >> nRelays;
    if (nRelays == 0) {
		cout << "Invalid line 1. Number required.";
		return 3;
    }
    cout << "Relays number: " << nRelays << "\n";
    int32_t parents[nRelays];
    int32_t children[nRelays];
    int32_t index = 0;

	while (getline(file, line)) {
        stringstream lineStream = stringstream(line);
        int32_t parent;
        int32_t child;
        lineStream >> parent >> child;
        parents[index] = parent;
        children[index] = child;
        index++;
	}
	file.close();
    cout << "Read rows: " << index;

	/*************************************************************************
	 Finding the viral potential
    **************************************************************************/
    int32_t previousPoint = -1;
    // Let's find the farthest point
    // Start from any point (for instance from the first)
    propagationResult firstPropagationResult = propagate(parents[0],
                                                         previousPoint,
                                                         parents,
                                                         children,
                                                         nRelays);
    // Now find the biggest distance on the net
    // Start from the recently found farthest point
    cout << "\nFarthest point: " << firstPropagationResult.farthestPoint;
    propagationResult secondPropagationResult = propagate(firstPropagationResult.farthestPoint,
                                                          previousPoint,
                                                          parents,
                                                          children,
                                                          nRelays);
    // The vantage point is in the middle of the longest route
    int32_t depth = secondPropagationResult.depth;
    int32_t fastestPropagation = depth / 2;
    cout << "\nFastest propagation: " << fastestPropagation;
    return 0;
}
