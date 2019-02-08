/* -----------
 * project2.c
 * Project for CSCI 2021, Fall 2018, Professor Chris Dovolis
 * Written by Ry Wiese
 * This program will get the interger value from the user then change to the binary version and store it.
 * operate a variety of bitwise operations such as add, subtraction, and, or, not, left shift, and right shift. 
 * Kyeongtak Han
 * x500: han00127
 * ---------- */

#include <stdio.h>
#include <stdlib.h>


#define LEN 16
typedef char* reg;

void add(reg reg1, reg reg2, reg result_reg);
void subtract(reg reg1, reg reg2, reg result_reg);
void and(reg reg1, reg reg2, reg result_reg);
void or(reg reg1, reg reg2, reg result_reg);
void not(reg reg1, reg result_reg);
void logical_left_shift(reg reg1, int n, reg result_reg);
void arithmetic_right_shift(reg reg1, int n, reg result_reg);

int str_len(char* s);
int power(int x, int n);
void itoreg(int a, reg r);
short int regtoi(reg r);
void print_status(reg rega, reg regb, reg regc, reg regd);
void run_tests(reg rega, reg regb, reg regc, reg regd);

int main(int argc, char** argv) {
    //check for correct number of command line arguments
    if(argc != 3) {
        printf("Please enter exactly 2 integers as command line arguments.\n");
        printf("You entered %d integers.\n", argc - 1);
        exit(0);
    }

    //assign command line arguments to short ints
    short int a = atoi(argv[1]);
    short int b = atoi(argv[2]);
    short int c = 0;
    short int d = 0;

    //create four 16 bit registers
    reg rega = malloc((LEN + 1) * sizeof(char));
    reg regb = malloc((LEN + 1) * sizeof(char));
    reg regc = malloc((LEN + 1) * sizeof(char));
    reg regd = malloc((LEN + 1) * sizeof(char));

    //initialze the registers with the ints provided by the command line args
    itoreg(a, rega);
    itoreg(b, regb);
    itoreg(c, regc);
    itoreg(d, regd);

    print_status(rega, regb, regc, regd);
    printf("\n");
    run_tests(rega, regb, regc, regd);

    //always free malloced memory
    free(rega);
    free(regb);
    free(regc);
    free(regd);

    return 0;
}

// This is method that operate the add function. It will access each index then compare the value then store the correct value in result_reg.
// There are three cases that will occur. '0'&& '0', '0' && '1' or vice versa, '1' && '1'
void add(reg reg1, reg reg2, reg result_reg) {	
	int carry = 0;   //This will check out the case of '1' + '1'. If so carry will be added to next one. 
	for(int i = LEN -1; i >= 0; i--){
		// first case '0' and '0'
		if(reg1[i] == '0' && reg2[i] == '0'){
			if(carry == 1){
				result_reg[i] = '1';
				carry = 0;
			}else{
				result_reg[i] = '0';
				carry = 0;
			}
		}
  		// second case '1' and '1'
		else if(reg1[i] == '1' && reg2[i] == '1'){
			if(carry == 1){
				result_reg[i] = '1';
				carry =1;
			}else{
				result_reg[i] = '0';
				carry = 1;
			}
		}
                // third case '0' and '1' or '1' or '0'
		else{
			if(carry == 1){
				result_reg[i] = '0';
				carry = 1;
			}else{
				result_reg[i] = '1';
				carry = 0;
			}
		}
	}
	result_reg[LEN] = '\0';
}

/* The method that operates the subtraction using just bitwise operator and my another method of which add, not and the property of itoreg method.
   To preserve the original binary value, I used the two temps, temp and temp2.
     1. Take not function into reg2 then stored that value in temp2 (Inverting)
     2. Using the properity of Integer to Register method
     3. Add 1's complement and binary 1  to make 2's complement.
     4. Adding the result of 2's complement and reg1.
*/
void subtract(reg reg1, reg reg2, reg result_reg) {
	reg temp = malloc((LEN + 1) * sizeof(char));     // To presever the original value 
	reg temp2 = malloc((LEN + 1) * sizeof(char));    // To presever the original value 
        not(reg2, temp2);  //1's comlement of reg2 then store the value in temp2
	itoreg(1, temp);  // 0000000000000001 
	add(temp2, temp, temp2); // add temp2(not reg2) and 0000000000000001 for 2's complement
	add(reg1,temp2,result_reg); // add the reg1 and temp2(2's complement of not reg2) then store it in the result_reg
}


// The method that do bitwise operation, and. If '1' and '1' -> '1'  else '0'. Store the value in result_reg.
void and(reg reg1, reg reg2, reg result_reg) {
	int i;
	for(i = LEN -1; i>= 0; i--){
		if(reg1[i] == '1' && reg2[i] == '1'){result_reg[i] = '1';}
		else{result_reg[i] ='0';}
	}
	result_reg[LEN] = '\0';
}


// The method that do bitwise operation, or. '0' and '0' is the only case that can result in '0'. Otherwise '1'. Store the value in result_reg.
void or(reg reg1, reg reg2, reg result_reg) {
	int i;
	for(i = LEN -1; i>= 0; i--){
		if(reg1[i] == '0' && reg2[i] == '0'){result_reg[i] = '0';}
		else{result_reg[i] ='1';}
	}
	result_reg[LEN] = '\0';
}


// The method that do bitwise operation, not. If it's '1' --> '0'. Otherwise '0'. Store the value in result_reg.
void not(reg reg1, reg result_reg) {
	int i;
	for(i = LEN-1; i>= 0; i--){
		if(reg1[i] == '1'){result_reg[i] ='0';}
		else{result_reg[i] = '1';}
	}
	result_reg[LEN] = '\0';
}


/* The method that do logical left shift ex) 11001010  --> 1001 0100 then stores the value in result_reg.
   -pre n=0 result_reg equal to reg1. */
void logical_left_shift(reg reg1, int n, reg result_reg) {
	int i;
	int j = 0;  // Control part in order to decide the place to put the 0's.

	// copy the register1
	for(i = LEN-1; i >= 0; i --){
		result_reg[i] = reg1[i];
	}

        // It will iterate the same amount as the int n (parameter)
	while(n > 0){
		for(i=0; i < LEN-1 - j; i++){  // passing from the last index of the binary
			result_reg[i] = result_reg[i +1];
		}
	result_reg[LEN-1-j] = '0'; // Put the zero at the end. It also keep moving to left
	n--;  // Control the how many times it will iterate.
	j++;  // Skip the place.
	}
	result_reg[LEN] = '\0';
}


/* The method that do logical right shift ex) 11001010  --> 11100101 then stores the value in result_reg.
   -pre n=0 result_reg equal to reg1. */
void arithmetic_right_shift(reg reg1, int n, reg result_reg) {
	int i;
	int j = 0;
        // Copy the register1.
	for(i = LEN-1; i >= 0; i --){
		result_reg[i] = reg1[i];
	}
	// if the first is '1' --> It will keep the value '1'. because it need to keep the minus.
	if(reg1[0] == '1'){
		while(n > 0){
			for(i = LEN -2; i > 0; i--){
				result_reg[i] = result_reg[i -1]; // assgined the previous value to current place. left to right.
			}
		result_reg[0] = '1';  // Keep push the '1' in the first place of binary.
		n--;
 		j++;
		}
	}
	else{
		// This will be iterated exactly same times as the int n. 
		while(n > 0){
			for(i=LEN-2; i > 0; i--){
				result_reg[i] = result_reg[i - 1];
			}
		result_reg[j] = '0';
		n--; // Control the how many times while loop will iterate.
		j++; // Skip the place.
		}
	}
	result_reg[LEN] = '\0';
}


//finds the length of s
int str_len(char* s) {
    int l = 0;
    while(s[l] != '\0')
        l++;
    return l-1;
}

//returns x^n
int power(int x, int n) {
    if(n <= 0)
        return 1;
    else
        return x * power(x, n-1);
}

//short for integer to register
//stores the 2s complement version of a in r
void itoreg(int a, reg r) {
    int i = 0;
    int p = LEN - 1;

    //handle whether a is positive or negative
    if(a >= 0) {
        r[i] = '0';
    }
    else {
        r[i] = '1';
        a += power(2, p);
    }
    i++;
    p--;

    //fill in the rest of the register
    while(i < LEN) {
        if(a/power(2, p)) {
            r[i] = '1';
            a -= power(2, p);
        }
        else {
            r[i] = '0';
        }
        i++;
        p--;
    }
    r[LEN] = '\0';
}

//short for register to integer
//returns the integer version of r
short int regtoi(reg r) {
    int p = LEN - 1;
    int i = 0;
    int a = -1 * (r[i] - 48) * power(2, p);
    i++;
    p--;
    while(i < LEN) {
        a += (r[i] - 48) * power(2, p);
        i++;
        p--;
    }
    return a;
}

//prints the values in the registers
void print_status(reg rega, reg regb, reg regc, reg regd) {
    printf("Register status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", regtoi(rega), regtoi(regb), regtoi(regc), regtoi(regd));
    printf("rega: %s\nregb: %s\nregc: %s\nregd: %s\n", rega, regb, regc, regd);
}

//run a series of tests to see if your functions work properly
void run_tests(reg rega, reg regb, reg regc, reg regd) {
    signed short int a, b, c, d;

    //Test add(rega, regb, regc)
    printf("Adding rega to regb and storing the result in regc...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    add(rega, regb, regc);
    c = a + b;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test add(rega, regb, regb)
    printf("Adding rega to regb and storing the result in regb...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    add(rega, regb, regb);
    b = a + b;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test add(rega, regc, rega)
    printf("Adding rega to regc and storing the result in rega...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    add(rega, regc, rega);
    a = a + c;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test subtract(regb, rega, regd)
    printf("Subtracting rega from regb and storing the result in regd...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    subtract(regb, rega, regd);
    d = b - a;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test subtract(regd, regb, regb)
    printf("Subtracting regb from regd and storing the result in regb...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    subtract(regd, regb, regb);
    b = d - b;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test subtract(rega, regc, rega)   subtract(rega, regc, rega);   a= a-c
    printf("Subtracting regc from rega and storing the result in rega...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    subtract(rega, regc, rega);
    a = a - c;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test and(rega, regc, regd)
    printf("Bitwise and-ing rega and regc and storing the result in regd...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    and(rega, regc, regd);
    d = a & c;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test and(regc, regd, rega)
    printf("Bitwise and-ing regc and regd and storing the result in rega...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    and(regc, regd, rega);
    a = c & d;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test and(rega, regb, rega)
    printf("Bitwise and-ing rega and regb and storing the result in rega...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    and(rega, regb, rega);
    a = a & b;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test or(rega, regb, regd)
    printf("Bitwise or-ing rega and regb and storing the result in regd...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    or(rega, regb, regd);
    d = a | b;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test or(regc, rega, rega)
    printf("Bitwise or-ing regc and rega and storing the result in rega...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    or(regc, rega, rega);
    a = c | a;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test or(regb, regc, regb)
    printf("Bitwise or-ing regb and regc and storing the result in regb...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    or(regb, regc, regb);
    b = b | c;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test not(regb, rega)
    printf("Bitwise not-ing regb and storing the result in rega...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    not(regb, rega);
    a = ~b;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test not(regd, regd)
    printf("Bitwise not-ing regd and storing the result in regd...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    not(regd, regd);
    d = ~d;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test logical_left_shift(rega, 4, regc)
    printf("Logical left shifting rega by 4 and storing the result in regc...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    logical_left_shift(rega, 4, regc);
    c = a << 4;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test logical_left_shift(regb, 6, regb)
    printf("Logical left shifting regb by 6 and storing the result in regb...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    logical_left_shift(regb, 6, regb);
    b = b << 6;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test arithmetic_right_shift(regc, 2, regb)
    printf("Arithmetic right shifting regc by 2 and storing the result in regb...\n");
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    arithmetic_right_shift(regc, 2, regb);
    b = c >> 2;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");

    //Test arithmetic_right_shift(rega, 7, rega)
    printf("Arithmetic right shifting rega by 7 using a negative number and storing the result in rega...\n");
    itoreg(-3945, rega);
    a = regtoi(rega);
    b = regtoi(regb);
    c = regtoi(regc);
    d = regtoi(regd);
    arithmetic_right_shift(rega, 7, rega);
    a = a >> 7;
    printf("Expected status:\n");
    printf("rega: %d\nregb: %d\nregc: %d\nregd: %d\n", a, b, c, d);
    print_status(rega, regb, regc, regd);
    if(regtoi(rega)==a && regtoi(regb)==b && regtoi(regc)==c && regtoi(regd)==d)
        printf("Correct!\n\n");
    else printf("Wrong!\n\n");
}
