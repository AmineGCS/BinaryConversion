/**************************
* Name: Amine Ghoorchiyani
* ID: 50553657
* How to compile: g++ binaryConversion.cpp
**************************/

#include <cmath>
#include <iostream> 
#include <string>
#include <vector>

using namespace std;

const int SIZE = 32;
const int SIG_SIZE = 23; // size of significant
const int EXP_SIZE = 8;
const double EPSILON = 2e-38;

void get_binary(int [], float, int);
void from_decimal_to_floating();
void from_floating_to_decimal();
void determine_hex(char [], int []);
void determine_binary(char [], int []);
void print_menu();

int main(){
    
    int menu;
   
    print_menu(); 
    cout<< "Enter a number to select the menu: "; 
    cin >> menu; 

    do {

        if(menu == 1){
            from_decimal_to_floating();
        }
        else if (menu == 2){
            from_floating_to_decimal();
        }   
        else if (menu == 0){
            printf("Bye!!!\n");
            exit(0); 
        }
        else {
           cout << "wrong input!!!" << endl;
        }

        print_menu();
        cout << "Enter a number to select the menu: "; 
        cin >> menu; 
     
    }while (1);

    return 0;
}

void get_binary(int binary[], float number, int size)
{
    // m_of_2 represents multiplication of 2
    float m_of_2 = 1.0;
    int index = 0;
    bool stop = false, negative = false;
    
    //write the code here
    
    if(number < 0)
    {
        number *= -1; // first treat is as a positive integer 
        negative = true;
    }

        // find a right number of mulptiplication of two that is <= number
        while(number > m_of_2 && !stop) 
        {
            // less than number and cannot be underflown
            if((m_of_2 * 2) < number || fabs((m_of_2 * 2) - number) < EPSILON)
            {
                m_of_2 *= 2;
                index++; // keeps track of two to the nth power
            }
            else // m_of_2 > number
            {
                stop = true;
            }
        }
            
        do // set 1 to the corret position of binary representation
        {
            if((number - m_of_2) > 0 || fabs(number - m_of_2) < EPSILON) 
            {
                number -= m_of_2;
                //  set 1 to the current nth power
                binary[size - index - 1] = 1; 
            }
            m_of_2 /= 2; // two to the n - 1 power
            index--; 
        } while(fabs(m_of_2) > EPSILON);

    
    if(negative)
    {
        for(int index = 0; index < size; index++)
        {
            if(binary[index] == 0) // flip 0s and 1s
            {
                binary[index] = 1;
            }
            else // binary[index] == 1
            {
                binary[index] = 0;
            }
        }
        
        if(binary[size - 1] == 0) // + 1 at first digit
        {
            binary[size - 1] = 1;
        }
        else // binary[size - 1] == 1
        {
            // adjust the digits
            int index = size - 1;
            while(binary[index] == 1)
            {
                binary[index] = 0;
                index--;
            }
            if(binary[index] == 0)
            {
                binary[index] = 1;
            }  
        } 
    }
}

void from_decimal_to_floating(){
    char number[10];
    float decimal = 0, stored;
    // int_temp stores integer part for the significant bits
    // dec_temp stores decimal part for the significant bits
    // integer stores integer part of floating point number
    int binary[SIZE] = {0}, int_temp[256] = {0}, size = 0; 
    vector<int> dec_temp(SIG_SIZE);
    bool d_only = true; // assume that there is not integer part so only decimal
    cout << "Input decimal to convert a hexa decimal number (e.g., 0.4375): ";
    cin >> decimal;
    stored = decimal; // store the information
    //write the code here
    
    if(fabs(decimal) > EPSILON)
    {
        if(decimal < 0)
        {
            decimal *= -1.0;
        }

        if(trunc(decimal) > 0)
        {
            get_binary(int_temp, trunc(decimal), 256); // get the integer part of binary
            d_only = false;
        }

        decimal -= trunc(decimal);

        bool quit = false;
        while(!quit) // until no calculation is possible or it uses all 23 bits
        {
            decimal *= 2;
        
            if(decimal > 1.0 || fabs(decimal - 1.0) < EPSILON)
            {
                if(size < SIG_SIZE)
                {
                    dec_temp[size] = 1;
                }
                else
                {
                    dec_temp.push_back(1);
                }
                decimal -= 1.0; // take only decimal part
            }
            else // integer < 0
            {
                if(size < SIG_SIZE)
                {
                    dec_temp[size] = 0;
                }
                else
                {
                    dec_temp.push_back(0);
                }
            }

            if(fabs(decimal) < EPSILON) // if the value is 1.0 then stop the loop
            {
                quit = true;
            }
            
            size++;
        }
        
        int exponent = 0, dec_index = 0, int_index = 0;  // expoonent is a running total for expenent part
        bool stop = false;

        if(d_only) // only decimal part
        {
            // normalize for decimal
            if(dec_temp[dec_index] == 1)
            {
                exponent--; 
                dec_index++;
            }
            else
            {
                dec_index++;
                exponent--;
                while(!stop && dec_index < static_cast<int>(dec_temp.size())) // find 1 
                {
                    if(dec_temp[dec_index] == 1)
                    {
                        stop = true;
                    }
                    exponent--;
                    dec_index++;
                }
            }
        }
        else // include integer part
        {
            // normalize for integer part
            while(!stop && int_index < 256)
            {
                if(int_temp[int_index] == 1)
                {
                    stop = true;
                }
                if(!stop)
                {
                    int_index++;
                }
            }
            exponent = 256 - int_index - 1;
        }
        
        if(stored < 0) // determine the sign bit
        {
            binary[0] = 1;
        }
        else
        {
            binary[0] = 0;
        }
        
        int exp_array[EXP_SIZE] = {0};
        exponent += 127; // + bias
        get_binary(exp_array, exponent, EXP_SIZE);


        for(int index_1 = 0, index_2 = 1; index_1 < EXP_SIZE && index_2 < 9; index_1++, index_2++)
        {
            binary[index_2] = exp_array[index_1]; // store the exponential part
        }

        if(d_only)
        {
            for(int index_1 = dec_index, index_2 = 9; index_1 < static_cast<int>(dec_temp.size())
                && index_2 < SIZE; index_1++, index_2++)
            {
                binary[index_2] = dec_temp[index_1]; // store decimal part
            }
        }
        else // include integer part
        {
            // notmalized already so no need to calculate the intege part 
            // ex) 1.01010
            if(int_index == 255) 
            {
                for(int index_1 = 9, index_2 = 0; index_1 < SIZE 
                    && index_2 < SIG_SIZE; index_1++, index_2++)
                {
                    binary[index_1] = dec_temp[index_2];
                }
            }
            // normalize the integer part
            else
            {
                int binary_index = 9;
                while(int_index + 1 != 256) // integer part
                {
                    binary[binary_index] = int_temp[int_index + 1];
                    int_index++;
                    binary_index++;
                }

                for(int index = 0; index < SIG_SIZE && binary_index < SIZE; index++) // decimal part
                {
                    binary[binary_index] = dec_temp[index];
                    binary_index++;
                }
            }
        }
    }

    determine_hex(number, binary); // convert i=binary to hexadecimal
  
    //end of code 
    cout << stored << "'s floating point hexa decimal : " << number; 
    cout << endl << "[format]: 0.4375's floating point hexa decimal : 3EE00000" << endl << endl;
} 

void from_floating_to_decimal(){
    char number[10];
    float decimal = 0, decimal_max = 0.5;
    int binary[SIZE] = {0}, sign_bit, exponent = 0, exponent_max = 128;
    cout << "Input hexa decimal to convert a decimal number (e.g., 3EE00000): ";
    cin >> number;
    string check_if_0(number);
    //write the code here
    
    if(check_if_0 != "00000000")
    {
        determine_binary(number, binary); // convert hexadecimal to binary

        if(binary[0] == 0) // sign is positive
        {
            sign_bit = 1;
        }
        else // sign is negative
        {
            sign_bit = -1;
        }
    
        for(int index = 1; index < 9; index++) // get exponent
        {
            if(binary[index] == 1)
            {
                exponent += exponent_max;
            }
            exponent_max /= 2;
        }
        exponent -= 127; // - bias

        for(int index = 9; index < SIZE; index++) // get significand
        {
            if(binary[index] == 1)
            {
                decimal += decimal_max;
            }
            decimal_max /= 2.0;
        }
        decimal += 1; // + hidden bit

        decimal = sign_bit * decimal * pow(2, exponent); // multiply all to get the decimal value
    }
   
    //end of code 
    cout << number << "'s decimal : " << decimal ; 
    cout << endl << "[format]: 3EE00000's decimal : 0.4375" << endl << endl;
} 

void determine_hex(char number[], int binary[])
{
    int count = 0;

    for(int index = 0; index < 8; index++)
    {
        char temp[4];
        for(int i = 0; i < 4; i++) // get 4 bits
        {
            temp[i] = (to_string(binary[count]))[0];
            count++;
        }
        string s(temp);
        s.resize(4);
        // compare it with each case
        if(s == "0000")
        {
            number[index] = '0';
        }
        else if(s == "0001")
        {
            number[index] = '1';
        }
        else if(s == "0010")
        {
            number[index] = '2';
        }
        else if(s == "0011")
        {
            number[index] = '3';
        }
        else if(s == "0100")
        {
            number[index] = '4';
        }
        else if(s == "0101")
        {
            number[index] = '5';
        }
        else if(s == "0110")
        {
            number[index] = '6';
        }
        else if(s == "0111")
        {
            number[index] = '7';
        }
        else if(s == "1000")
        {
            number[index] = '8';
        }
        else if(s == "1001")
        {
            number[index] = '9';
        }
        else if(s == "1010")
        {
            number[index] = 'A';
        }
        else if(s == "1011")
        {
            number[index] = 'B';
        }
        else if(s == "1100")
        {
            number[index] = 'C';
        }
        else if(s == "1101")
        {
            number[index] = 'D';
        }
        else if(s == "1110")
        {
            number[index] = 'E';
        }
        else if(s == "1111")
        {
            number[index] = 'F';
        }
    }
}

void determine_binary(char number[], int binary[])
{
    char element;
    int count = 0;
    for(int index = 0; index < 8; index++)
    {
        element = number[index]; // read one character from hexadecimal number
        
        // convert it into binary
        if(element == '0')
        {
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == '1')
        {
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
        }
        else if(element == '2')
        {
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == '3')
        {
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
        }
        else if(element == '4')
        {
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == '5')
        {
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
        }
        else if(element == '6')
        {
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == '7')
        {
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
        }
        else if(element == '8')
        {
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == '9')
        {
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
        }
        else if(element == 'A')
        {
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == 'B')
        {
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
        }
        else if(element == 'C')
        {
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == 'D')
        {
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
            binary[count] = 1;
            count++;
        }
        else if(element == 'E')
        {
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 0;
            count++;
        }
        else if(element == 'F')
        {
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
            binary[count] = 1;
            count++;
        }
    }
}

void print_menu(){

    cout <<"***************************************************************" << endl; 
    cout <<"******************    Homwwork 2: Menu    *********************" << endl;
    cout <<"*  1. Convert Decimal number to floating point representation *" << endl;
    cout <<"*  2. Convert Floating point representation to Decimal number *" << endl;
    cout <<"*  0. Exit the program                                        *" << endl;
    cout <<"***************************************************************" << endl;

}
