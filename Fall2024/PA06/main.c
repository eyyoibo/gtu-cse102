#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants for gate types
#define INPUT 0
#define OUTPUT 1
#define AND 2
#define OR 3
#define NOT 4
#define FLIPFLOP 5

// Function pointer type for gate evaluation functions
typedef int (*GateFunction)(int* inputs, int n, int* extra_state);

// Struct representing a logic gate
typedef struct Gate {
    int type;                          // The type of the gate (e.g., INPUT, OUTPUT, AND, etc.)
    char name[20];                     // Name of the gate, unique in the circuit
    struct Gate** input_gates;         // Array of pointers to input gates (dynamically allocated)
    int num_inputs;                    // Number of input gates
    int output;                        // Current output value of the gate
    int former_out;                    // Previous output (used for FLIPFLOP gates)
    int evaluated;                     // Flag indicating whether the gate has been evaluated
    GateFunction function;             // Pointer to the function defining the gate's logic
} Gate;

// AND gate function: returns 1 if all inputs are 1, otherwise 0
int and_function(int* inputs, int n, int* extra_state) {
    (void)extra_state; // Unused parameter
    for (int i = 0; i < n; i++) {
        if (!inputs[i]) {
            return 0;
        }
    }
    return 1;
}

// OR gate function: returns 1 if any input is 1, otherwise 0
int or_function(int* inputs, int n, int* extra_state) {
    (void)extra_state; // Unused parameter
    for (int i = 0; i < n; i++) {
        if (inputs[i]) {
            return 1;
        }
    }
    return 0;
}

// NOT gate function: returns the negation of the input
int not_function(int* inputs, int n, int* extra_state) {
    (void)extra_state; // Unused parameter
    return !inputs[0];
}

// Flipflop gate function: toggles output based on input and former state
int flipflop_function(int* inputs, int n, int* former_out) {
    int result = inputs[0] ? !(*former_out) : *former_out;
    *former_out = result; // Update the former state for the next evaluation
    return result;
}

// Array to map gate types to their respective functions
GateFunction gate_functions[] = {
    NULL,                // INPUT gates don't have a specific function
    NULL,                // OUTPUT gates don't have a specific function
    and_function,        // AND gate function
    or_function,         // OR gate function
    not_function,        // NOT gate function
    flipflop_function    // Flipflop gate function
};

// Function to create a new gate
Gate* create_gate(int type, char* name) {
    // Allocate memory for a new gate
    Gate* new_gate = (Gate*)malloc(sizeof(Gate));

    // Initialize gate attributes
    new_gate->type = type;
    strcpy(new_gate->name, name);
    new_gate->num_inputs = 0;
    new_gate->input_gates = NULL;
    new_gate->output = 0;
    new_gate->former_out = 0;
    new_gate->evaluated = 0;
    new_gate->function = (type >= AND && type <= FLIPFLOP) ? gate_functions[type] : NULL;
    return new_gate;
}

// Function to connect one gate's output to another gate's input
void connect_gates(Gate* from, Gate* to) {
    // Expand the input_gates array of the destination gate to accommodate a new input
    to->input_gates = (Gate**)realloc(to->input_gates, (to->num_inputs + 1) * sizeof(Gate*));

    // Add the source gate to the input list
    to->input_gates[to->num_inputs++] = from;
}

// Function to reset all gates before a new evaluation
void reset_gates(Gate** gates, int count) {
    for (int i = 0; i < count; i++) {
        gates[i]->evaluated = 0; // Reset the evaluated flag for all gates
    }
}

// Function to evaluate a gate recursively
int evaluate_gate(Gate* gate) {
    if (gate->evaluated) {
        return gate->output; // Return the cached output if already evaluated
    }

    // Collect inputs by evaluating connected input gates
    int* inputs = (int*)malloc(gate->num_inputs * sizeof(int));

    for (int i = 0; i < gate->num_inputs; i++) {
        inputs[i] = evaluate_gate(gate->input_gates[i]);
    }

    // Compute the output using the gate's logic function
    if (gate->function) {
        gate->output = gate->function(inputs, gate->num_inputs, &gate->former_out);
    } else {
        gate->output = (gate->type == OUTPUT) ? inputs[0] : 0;
    }

    gate->evaluated = 1; // Mark the gate as evaluated
    free(inputs);
    return gate->output;
}

// Function to map a string to its corresponding gate type
int get_gate_type(char* type_str) {
    if (strcmp(type_str, "INPUT") == 0) return INPUT;
    if (strcmp(type_str, "OUTPUT") == 0) return OUTPUT;
    if (strcmp(type_str, "AND") == 0) return AND;
    if (strcmp(type_str, "OR") == 0) return OR;
    if (strcmp(type_str, "NOT") == 0) return NOT;
    if (strcmp(type_str, "FLIPFLOP") == 0) return FLIPFLOP;
    return -1; // Invalid gate type
}

int main() {
    // Initialize arrays to store gates
    Gate** gates = NULL;
    int gate_count = 0;

    Gate** input_gates = NULL;
    int input_count = 0;

    Gate** output_gates = NULL;
    int output_count = 0;

    // Read and parse the circuit.txt file
    FILE* circuit_file = fopen("circuit.txt", "r");

    // Process each line in circuit.txt
    char line[32];
    while (fgets(line, sizeof(line), circuit_file)) {
        char type[10], name1[20], name2[20];
        if (sscanf(line, "GATE %s %s", type, name1) == 2) {
            // Create and store a new gate
            int gate_type = get_gate_type(type);
            if (gate_type == -1) continue;
            Gate* gate = create_gate(gate_type, name1);

            gates = (Gate**)realloc(gates, (gate_count + 1) * sizeof(Gate*));
            gates[gate_count++] = gate;

            // Categorize gates into input/output
            if (gate_type == INPUT) {
                input_gates = (Gate**)realloc(input_gates, (input_count + 1) * sizeof(Gate*));
                input_gates[input_count++] = gate;
            } else if (gate_type == OUTPUT) {
                output_gates = (Gate**)realloc(output_gates, (output_count + 1) * sizeof(Gate*));
                output_gates[output_count++] = gate;
            }
        } else if (sscanf(line, "CONNECTION %s %s", name1, name2) == 2) {
            // Connect gates based on their names
            Gate* from = NULL, *to = NULL;
            for (int i = 0; i < gate_count; i++) {
                if (strcmp(gates[i]->name, name1) == 0) from = gates[i];
                if (strcmp(gates[i]->name, name2) == 0) to = gates[i];
            }
            if (from && to) connect_gates(from, to);
        }
    }
    fclose(circuit_file);

    // Read and process the input.txt file
    FILE* input_file = fopen("input.txt", "r");

    while (fgets(line, sizeof(line), input_file)) {
        reset_gates(gates, gate_count); // Reset all gates before evaluating new inputs

        // Assign inputs to input gates
        for (int i = 0; i < input_count; i++) {
            input_gates[i]->output = line[i] - '0';
            input_gates[i]->evaluated = 1; // Mark input gates as evaluated
        }

        // Evaluate and print outputs
        for (int i = 0; i < output_count; i++) {
            printf("%d", evaluate_gate(output_gates[i]));
        }
        printf("\n");
    }
    fclose(input_file);

    // Free all allocated memory
    for (int i = 0; i < gate_count; i++) {
        free(gates[i]->input_gates);
        free(gates[i]);
    }
    free(gates);
    free(input_gates);
    free(output_gates);

    return 0;
}