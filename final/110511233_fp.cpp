#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

map<string, int> gateType = {
    {"NAND", 0}, {"AND", 1}, {"NOR", 2}, {"OR", 3}, {"XOR", 4}, {"XNOR", 5}, 
    {"INV", 6}, {"USD01", 7}, {"USD02", 8}, {"USD03", 9}, {"USD04", 10}, 
    {"USD05", 11}, {"USD06", 12}, {"USD07", 13}, {"USD08", 14}, {"USD09", 15}, {"USD10", 16} };

vector<int> gateCost = { 2, 3, 2, 3, 4, 4, 1, 5, 2, 3, 4, 5, 2, 3, 5, 2, 4 };
bool gateTruthTable[][4] = {
    {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 1, 0}, {1, 0, 0, 1},
    {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 1, 0, 0}, 
    {0, 1, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 1} };

struct Wire;
struct Node;

struct Wire {
    bool bitOnWire;
    Node *source;
    vector<Node*> destination;

    // string name; // for debug

    Wire() : bitOnWire(false), source(nullptr), destination(vector<Node*>()) {}
};

struct Node {
    int type;
    Wire *in1Wire, *in2Wire, *outWire;
    int delay;
    bool visited;

    // string name; // for debug
    
    Node() : type(-1), in1Wire(nullptr), in2Wire(nullptr), outWire(nullptr), delay(0), visited(0) {}
};

class CircuitGraph {
public:
    CircuitGraph() : nodes(map<string, Node>()), wires(map<string, Wire>()) {}
    
    void addExposedNet(const string& netName) {
        wires[netName] = Wire();
        // wires[netName].name = netName; // for debug
        exposedNets.push_back(netName);
    }

    void addWire(const string& netName) {
        wires[netName] = Wire();
        // wires[netName].name = netName; // for debug
    }

    void setInput(const string& netName) {
        inputs.push_back(netName);
    }

    void setOutput(const string& netName) {
        outputs.push_back(netName);
    }

    void addInstance(const string& elem, const string& inst) {
        Node node;
        node.type = gateType[elem];
        nodes[inst] = node;

        // nodes[inst].name = inst; // for debug
    }

    void setWireDestination(const string& netName, const string& inst, bool in) {
        Node* node = &nodes[inst];

        if (in) {
            node->in1Wire = &wires[netName];
        } else {
            node->in2Wire = &wires[netName];
        }

        wires[netName].destination.push_back(node);
    }

    void setWireSource(const string& netName, const string& inst) {
        Node* node = &nodes[inst];

        node->outWire = &wires[netName];

        wires[netName].source = node;
    }

    void prepare() {
        topologicalSort();

        // set output order 
        // use the order of exposedNets
        for (auto& net: exposedNets) {
            if (find(outputs.begin(), outputs.end(), net) != outputs.end()) {
                outputOrder.push_back(net);
            }
        }
    }
    
    int getCriticalPathDelay() {
        int maxDelay = 0;

        for (auto& node: nodes) {
            node.second.delay = 0;
        }

        stack<Node*> tmp = topologicalStack;

        while (!tmp.empty()) {
            Node* node = tmp.top();
            tmp.pop();
            node->delay += gateCost[node->type];

            for (auto& dest: node->outWire->destination) {
                if (dest == nullptr) continue;
                dest->delay = max(dest->delay, node->delay);
            }

            if (node->outWire->destination.empty()) {
                maxDelay = max(maxDelay, node->delay);
            }
            // maxDelay = max(maxDelay, node->delay);
        }

        return maxDelay;
    }

    vector<bool> evaluate(vector<bool> x) {
        vector<bool> result;    
        resetBitOnWire();

        // for (auto& input: inputOrder) {
        //     wires[input].bitOnWire = x & 1;
        //     x >>= 1;
        // }

        for (unsigned i = 0; i < inputOrder.size(); i++) {
            wires[inputOrder[i]].bitOnWire = x[i];
        }

        stack<Node*> tmp = topologicalStack;

        while (!tmp.empty()) {
            Node* node = tmp.top();
            tmp.pop();

            int index = (node->in1Wire ? node->in1Wire->bitOnWire * 2 : 0) + 
                        (node->in2Wire ? node->in2Wire->bitOnWire     : 0) ;

            node->outWire->bitOnWire = gateTruthTable[node->type][index];
        }

        for (auto& output: outputOrder) {
            result.push_back(wires[output].bitOnWire);
        }

        return result;
    }

    void setInputOrder(const vector<string>& _inputOrder) {
        inputOrder = _inputOrder;
        // reverse(inputOrder.begin(), inputOrder.end());
    }

private:  
    void topologicalSort() {
        // reset the stack
        topologicalStack = stack<Node*>();

        // traverse nodes and reset the visited flag
        for (auto& node: nodes) {
            node.second.visited = false;
        }

        // topological sort
        for (auto& input: inputs) {
            topologicalSortDFS(&wires[input]);
        }
    }

    void topologicalSortDFS(Wire* wire) {
        for (auto& node: wire->destination) {
            if (!node->visited) {
                node->visited = true;
                topologicalSortDFS(node->outWire);
            }
        }

        if (wire->source != nullptr) {
            topologicalStack.push(wire->source);
        }
    }

    void resetBitOnWire() {
        for (auto& wire: wires) {
            wire.second.bitOnWire = false;
        }
    }

    vector<string> exposedNets;
    vector<string> inputs;
    vector<string> inputOrder;
    vector<string> outputs;
    vector<string> outputOrder;

    map<string, Node> nodes;
    map<string, Wire> wires;

    stack<Node*> topologicalStack;
};


string& trim(string &s) {
    if (s.empty()) { return s; }

    s.erase(0, s.find_first_not_of(" \t\r"));
    s.erase(s.find_last_not_of(" \t\r") + 1);
    return s;
}

int main() {
    CircuitGraph circuit;
    string tmp;

    stringstream ss;
    ss.str("");
    ss.clear();

    getline(cin, tmp);
    // parse net names 
    int start = tmp.find('(') + 1, end = tmp.find(')');
    tmp = tmp.substr(start, end - start);

    // split by comma
    string net;
    while (tmp.find(',') != string::npos) {
        net = tmp.substr(0, tmp.find(','));
        // remove space
        circuit.addExposedNet(trim(net));
        tmp = tmp.substr(tmp.find(',') + 1);
    }
    net = tmp.substr(0, tmp.find(')'));
    circuit.addExposedNet(trim(net));

    while (getline(cin, tmp)) {
        trim(tmp);
        
        if (tmp == "endmodule") {
            break;
        } else if (tmp == "") {
            continue;
        } else if (tmp.find("input") == 0) {
            tmp = tmp.substr(5);

            // split by comma or semicolon 
            while(tmp.find(',') != string::npos) {
                net = tmp.substr(0, tmp.find(','));
                circuit.setInput(trim(net));
                tmp = tmp.substr(tmp.find(',') + 1);
            }
            // last one split by semicolon
            net = tmp.substr(0, tmp.find(';'));
            circuit.setInput(trim(net));

        } else if (tmp.find("output") == 0) {
            tmp = tmp.substr(6);

            // split by comma or semicolon 
            while(tmp.find(',') != string::npos) {
                net = tmp.substr(0, tmp.find(','));
                circuit.setOutput(trim(net));
                tmp = tmp.substr(tmp.find(',') + 1);
            }
            // last one split by semicolon
            net = tmp.substr(0, tmp.find(';'));
            circuit.setOutput(trim(net));

        } else if (tmp.find("wire") == 0) {
            tmp = tmp.substr(5);

            // split by comma or semicolon 
            while(tmp.find(',') != string::npos) {
                net = tmp.substr(0, tmp.find(','));
                circuit.addWire(trim(net));
                tmp = tmp.substr(tmp.find(',') + 1);
            }
            // last one split by semicolon
            net = tmp.substr(0, tmp.find(';'));
            circuit.addWire(trim(net));

        } else { // instance
            string inst, elem, A1, A2, ZN, name;
            // INV g1(.ZN(n4), .I(n1));

            // find the first space
            start = tmp.find(' ');
            elem = tmp.substr(0, start);
            trim(elem);
            tmp = tmp.substr(start + 1);

            // find the first (
            start = tmp.find('(');
            inst = tmp.substr(0, start);
            trim(inst);
            tmp = tmp.substr(start + 1);

            while ((start = tmp.find('.')) != string::npos) {
                // find the name between . and (
                start += 1;
                end = tmp.find('(');
                name = tmp.substr(start, end - start);
                trim(name);
                tmp = tmp.substr(end + 1);

                // find the first )
                end = tmp.find(')');
                if (name == "A1") {
                    A1 = tmp.substr(0, end);
                    trim(A1);
                } else if (name == "A2" || name == "I") {
                    A2 = tmp.substr(0, end);
                    trim(A2);
                } else if (name == "ZN") {
                    ZN = tmp.substr(0, end);
                    trim(ZN);
                }
            }

            circuit.addInstance(elem, inst);
            if (A1 != "") circuit.setWireDestination(A1, inst, 1);
            circuit.setWireDestination(A2, inst, 0);
            circuit.setWireSource(ZN, inst);
        } 
    }

    circuit.prepare();
    // cout << circuit.getCriticalPathDelay() << endl;
    ss << circuit.getCriticalPathDelay() << endl;

    vector<string> inputNames;
    // using getline to read the input
    while (getline(cin, tmp)) {
        trim(tmp);
        if (tmp == ".end") {
            break;
        } else if (tmp == "") {
            continue;
        } else if (tmp.find("input") == 0) {
            tmp = tmp.substr(5);

            // split by comma or semicolon 
            while(tmp.find(',') != string::npos) {
                net = tmp.substr(0, tmp.find(','));
                inputNames.push_back(trim(net));
                tmp = tmp.substr(tmp.find(',') + 1);
            }
            inputNames.push_back(trim(tmp));

            circuit.setInputOrder(inputNames);
        } else {
            // unsigned long long int x = 0;
            vector<bool> x;
            string value;
            end = tmp.find(' ');
            do {
                value = tmp.substr(0, end);
                tmp = tmp.substr(end + 1);
                trim(tmp);
                // x <<= 1;
                // x |= (value == "1" ? 1 : 0);
                x.push_back(value == "1" ? 1 : 0);
            } while ((end = tmp.find(' ')) != string::npos);
            // x <<= 1;
            // x |= (tmp == "1" ? 1 : 0);
            x.push_back(value == "1" ? 1 : 0);

            for (auto y: circuit.evaluate(x)) {
                // cout << y << ' ';
                ss << y << ' ';
            }
            // cout << endl;
            ss << endl;
        }
    }
    
    cout << ss.str();
    return 0;
}