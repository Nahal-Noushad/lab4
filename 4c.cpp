#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

// Function to print real numbers in fixed-point notation
void print_real(float number, int fieldspace, int precision) {
    cout << setw(fieldspace) << fixed << setprecision(precision) << number << endl;
}

// Template function to update the values of a and b
template <typename T>
void update_scale(T& m1, T& m2, T m3 = 10) {
    T original_m1 = m1;
    T original_m2 = m2;

    // Update m1 and m2 according to the provided formula
    m1 = (original_m1 + original_m2) * m3;
    m2 = (original_m1 - original_m2) * m3;
}

int main(void) {
    float a, b;
    
    // Input two real numbers
    cout << "Please input two real numbers: \n";
    cin >> a >> b;

    // Print the original values of a and b
    cout << "Original values:" << endl;
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    // Call the function update_scale with ONLY the actual parameters a and b
    update_scale(a, b);

    // Print the updated values of a and b
    cout << "Updated values:" << endl;
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    return 0;
}
