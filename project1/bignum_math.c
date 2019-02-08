
/* -----------
 * bignum_math.c
 * Project for CSCI 2021, Fall 2018, Professor Chris Dovolis
 * Name: Kyeongtak Han
 * x500: han00127
 * ---------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define false 0;
#define true 1;

typedef int bool;

/*
 * Returns true if the given char is a digit from 0 to 9
 */
bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

/*
 * Returns true if lower alphabetic character
 */
bool is_lower_alphabetic(char c) {
	return c >= 'a' && c <= 'z';
}

/*
 * Returns true if upper alphabetic character
 */
bool is_upper_alphabetic(char c) {
	return c >= 'A' && c <= 'Z';
}

/*
 * Convert a string to an integer
 * returns 0 if it cannot be converted.
 */
int string_to_integer(char* input) {
	int result = 0;
	int length = strlen(input);
	int num_digits = length;
	int sign = 1;
	int i = 0;
	int factor = 1;

	if (input[0] == '-') {
		num_digits--;
		sign = -1;
	}

	for (i = 0; i < num_digits; i++, length--) {
		if (!is_digit(input[length-1])) {
			return 0;
		}
		if (i > 0) factor*=10;
		result += (input[length-1] - '0') * factor;
	}

	return sign * result;
}

/*
 * Returns true if the given base is valid.
 * that is: integers between 2 and 36
 */
bool valid_base(int base) {
	if(!(base >= 2 && base <= 36)) {
		return false;
	}
	return true;
}

/*
 * post - check the value whether it's in digits or alpabet.
 * This is the function that check out the valid input. Access the char * input Then,
 * it compares and checks out it valid input with using the is_digit function, is_alpabet functions and int base. 
 * it will return true or false value which defined at the top.
 */
bool valid_input(char* input, int base) {  	
	int valid_num; // Store the integer value to compare with the base. 
	for (int i = 0; input[i] != '\0'; i++) {	
		// This is for minus case. Pre-check to be able to handle the minus case. 		
		if (!is_digit(input[i]) && !is_lower_alphabetic(input[i]) && !is_upper_alphabetic(input[i])){
			return false;
		}

		if(is_digit(input[i])){
			if(input[i] < '0'){   
				return false;
			}
			else if (base <= 10){				
				if(input[i] - '0'  >= base){
					return false;
				}
			}
		}

		// it's alpabet 	
		else {

			if(base <= 10){
				return false;
			} 
			else{
				// set the valid_num whenever it's repeated to avoid the wrong value.
				valid_num = 0;
				if(is_lower_alphabetic(input[i])){ // lowercase
					// Use the fact that the char value has own integer value. 		   
					valid_num = input[i] - 'a' + 10;
					if(valid_num >= base){
						return false;
					}
				}
				else{ // Uppercase 
					valid_num = input[i] - 'A' + 10;
					if(valid_num >= base){
						return false;
					}
				}
			}
		}
	}
	return true;  
}	
	

/*
 * converts from an array of characters (string) to an array of integers
 */
int* string_to_integer_array(char* str) {
	int* result;
	int i, str_offset = 0;
		result = malloc((strlen(str) + 1) * sizeof(int));
		result[strlen(str)] = -1;
	for(i = str_offset; str[i] != '\0'; i++) {
		if(is_digit(str[i])) {
			result[i - str_offset] = str[i] - '0';
		} else if (is_lower_alphabetic(str[i])) {
			result[i - str_offset] = str[i] - 'a' + 10;
		} else if (is_upper_alphabetic(str[i])) {
			result[i - str_offset] = str[i] - 'A' + 10;
		} else {
			printf("I don't know how got to this point!\n");
		}
	}
	return result;
}

/*
 * finds the length of a bignum...
 * simply traverses the bignum until a negative number is found.
 */
int bignum_length(int* num) {
	int len = 0;
	while(num[len] >= 0) { len++; }
	return len;
}

/*
 * pre - array is not empty 
 * post - checks the base with char * int
 * This is the function that prints the digits and alpabet if the value over the 10. 
 * It should print only the lower case.
 */
void bignum_print(int* num) {
	int i;
	if(num == NULL) { return; }

	/* Handle negative numbers as you want */
	i = bignum_length(num);

	/* Then, print each digit */
	for(i = 0; num[i] >= 0; i++) {
		if(num[i] < 10){
			printf("%d", num[i]);
		}		
		else{
			char temp = (num[i] + 'a' -10);
			printf("%c", temp);
		}
	}
	printf("\n");
}

/*
 *	Helper for reversing the result that we built backward.
 *  see add(...) below
 */
void reverse(int* num) {
	int i, len = bignum_length(num);
	for(i = 0; i < len/2; i++) {
		int temp = num[i];
		num[i] = num[len-i-1];
		num[len-i-1] = temp;
	}
}

/*
 * addition of input1 and input2
 * PROVIDED FOR GUIDANCE
 */
int* add(int* input1, int* input2, int base) {
	int len1 = bignum_length(input1);
	int len2 = bignum_length(input2);
	int resultlength = ((len1 > len2)? len1 : len2) + 2;
	int* result = (int*) malloc (sizeof(int) * resultlength);
	int r = 0;
	int carry = 0;
	int sign = input1[len1];
	int num1, num2;

	len1--;    
	len2--;    

	while (len1 >= 0 || len2 >= 0) {
	        if (len1 >= 0) {
        	    num1 = input1[len1];
	        } else {
        	    num1 = 0;
        	}

		if (len2 >= 0) {
		    num2 = input2[len2];
		} else {
		    num2 = 0;
		}
		
		result[r] = (num1 + num2 + carry) % base;
		carry = (num1 + num2 + carry) / base;
		
		len1--;
		len2--;
		r++;
	}
	if (carry > 0) {
		result[r] = carry;
		r++;
    	}
	result[r] = sign;
	reverse(result);
	return result;
}


/*
 * This is the function that compared the input1 and input2.
 * By using the property of the function above (bignum_length).It will check out the length first. The reason --> having bigger size is always greater than other. 
 * If it has the same length, it will compare the value which is has same index number.
 * ex) {6,3,3,2}  {3,3,2}  --> NO matter what the arrays has the value, longer one is always greater than short one.
 * Otherwise, it'll check up the each value.
 * return true if input1 < input2, and false otherwise
 */   
bool less_than(int* input1, int* input2) {
	int len = bignum_length(input1);
	int len2 = bignum_length(input2);
	int i;
	if(len < len2){
		return true;
	}
	else if (len > len2){
		return false;
	}
	else{
		for(i = 0; i < len; i++){
			if(input1[i] > input2[i]){
				return false;
			}
			else if (input1[i] < input2[i]){
				return true;
			}
		}
		
	}	
	return false;
}

/*
 * This is the function that compared the input1 and input2.
 * By using the property of the function above (bignum_length). It will check out the length first. The reason --> having bigger size is always greater than other. 
 * If it has the same length, it will compare the value which is has same index number.
 * ex) {6,3,3,2}  {3,3,2}  --> NO matter what the arrays has the value, longer one is always greater than short one.
 * Otherwise, it'll check up the each value.
 * return true if input1 > input2, and false otherwise
 */
bool greater_than(int* input1, int* input2) {
	int len = bignum_length(input1);
	int len2 = bignum_length(input2);
	int i;
	if(len < len2){
		return false;
	}
	else if (len > len2){
		return true;
	}
	else{
		
		for(i =0; i < len; i++){
			if(input1[i] > input2[i]){
				return true;
			}
			else if (input1[i] < input2[i]){
				return false;
			}
		}
	}	
	return false;
}

/*
 * This is the function that compared the input1 and input2.
 * By using the property of the function above (bignum_length). It will check out the length first. The reason --> having bigger size is always greater than other. 
 * If it has the same length, it will compare the value which is has same index number.
 * ex) {6,3,3,2}  {3,3,2}  --> NO matter what the arrays has the value, longer one is always greater than short one.
 * Otherwise, it'll check up the each value.
 * return true if input1 == input2, and false otherwise
 */
bool equal_to(int* input1, int* input2) {
	int len = bignum_length(input1);
	int len2 = bignum_length(input2);
	int i;
	if(len < len2){
		return false;
	}
	else if (len > len2){
		return false;
	}
	else{
		
		for(i=0; i < len; i++){
			if(input1[i] != input2[i]){
				return false;
			}		
		}
	}	
	return true;
}


/*
 *   Basic Idea 
 *              622
 *            *  32
 *              ---
 * r -->     543210             
 *  i= 1       1244
 *  i= 2      1866*    --> * count variable to skip it. When i > 1, calculator skip the place as same amount as the count. Adding the value up and carry 
 *            19904
 * This is the function that multiply the two arrays without an overflow. 
 * return the result of multiplication.
 */
 
int* multiply(int* input1, int* input2, int base) {
	int len1 = bignum_length(input1);   
	int len2 = bignum_length(input2);
	int i, j,k;
	// small and big value will control the times of iteration of the for loop  ex)  622 * 32 --> {2} * {6}{2},{2},  3 * {6}{2}{2}
	int big, small;	  
	if(len1 >= len2){ 
		big = len1;
		small = len2;
	}else{
		big = len2;
		small = len1;
	}
	// The size of the product will be at least twice bigger than the big length.
	int resultlength = ((len1 > len2)? len1 : len2) * big; 
	// The size of product will be twice bigger than the big length
	int* result = (int*) malloc (sizeof(int) * resultlength); 
	int r = 0;   	
	int carry = 0;  
	int count = 1;  
	int sign = input1[len1]; 	
	int num1, num2;

	if (big == len1){
                 //                                                                   big   small
 		// This for loop will iterated the same amout of the small size. ex)  622 * 32 -->  2 * {6}{2},{2},  3 * {6}{2}{2}
		for(i = small -1; i >=0; i--){
			carry = 0;			
			num1 = input2[i];			
		       //This if statement check out the iterations. If second times of iteration, it assigned that r is equal to the place that it   skip. 
			if(count >= 2){
				// Get the value that how many place should it be skipped and assigned it to r. ex)  
				for(k =1; k < count; k++){
					r = k;                       
				}
			}
			// This will be iterated the same size of the big length
			for(j = big - 1; j >=0; j--){
				num2 = input1[j];
				// This will keep adding the remainder and shares of the num1 * num2
				result[r] += (num1 * num2 + carry) % base;
				carry = (num1 * num2 + carry) / base;
				// This if will check out the cumulated the value. if it greater than base, re-calculate the remainder and carrys.
				if(result[r] >= base){
					carry += result[r] / base; // keep adding the new result[r]'s carry in to new carry.
					result[r] = result[r] % base; // re-calculate the value.
				}
				r++;
			}
			// when the big size iteration ex) {6,2,2} is finished, add the last carry value at the end of the result. Then, r will be increase to put -1 value to wrap the result array up.
			if(carry > 0){
				result[r] = carry;
				r++;
			}
			count++;  // count will automatically be increased whenever outer iteration is finished. 
		}
	}
	// This is a exactly same logic and idea, excepts for the big and small value. 
	else{
		for(i = small-1; i >=0; i--){
			num1 = input1[i];
			if(count >= 2){
				for(k =1; k < count; k++){
					r = k;
				}
			}
			for(j = big-1; j >=0; j--){
				num2 = input2[j];
				result[r] += (num1 * num2 + carry) % base;
				carry = (num1 * num2 + carry) / base;
				if(result[r] >= base){
					carry += result[r] / base;
					result[r] = result[r] % base;
				}
				r++;
			}
			if(carry > 0){
				result[r] = carry;
				r++;
			}
			count++;
		}
	}
	
   	result[r] = sign;  //put the -1 at the end of the result array. 
	reverse(result);
	return result;
}

/*
 * This is the helper function to avoid the redundancy. 
 * the bool is defined at the top, true has 1 false has 0 value.
 * if the value has a 1 or 0, it wll print out true or false.
 */
void printf_bool(bool value){   
	if(value ==1){
		printf("true");
	}else{
		printf("false");
	}
	printf("\n");
}
/*
 * This is the function that operates the 4 kind of function, multiplication, 3 comparisons.
 * Prints the result.
 */
void perform_operation(int* input1, int* input2, char op, int base) {
	if(op == '+') {
		int* result = add(input1, input2, base);
		printf("Result: ");
		bignum_print(result);
		printf("\n");
		free(result);
	}
	else if (op == '*') {
		int* result = multiply(input1, input2, base);	
		printf("Result: ");
		bignum_print(result);
		printf("\n");
		free(result);
	}
	else if (op == '<') {
		bool result = less_than(input1, input2);
		printf("Result: ");
		printf_bool(result);
	}
	else if (op == '>') {
		bool result = greater_than(input1, input2);
		printf("Result: ");
		printf_bool(result);
	}
	else if(op == '='){
		bool result = equal_to(input1, input2);
		printf("Result: ");
		printf_bool(result);
	}

}

/*
 * Print to "stderr" and exit program
 */
void print_usage(char* name) {
	fprintf(stderr, "----------------------------------------------------\n");
	fprintf(stderr, "Usage: %s base input1 operation input2\n", name);
	fprintf(stderr, "base must be number between 2 and 36, inclusive\n");
	fprintf(stderr, "input1 and input2 are arbitrary-length integers\n");
	fprintf(stderr, "Permited operations are allowed '+', '*', '<', '>', and '='\n");
	fprintf(stderr, "----------------------------------------------------\n");
	exit(1);
}


/*
 * MAIN: Run the program and tests your functions.
 * sample command: ./bignum 4 12 + 13
 * Result: 31
 */
int main(int argc, char** argv) {

	int input_base;

	int* input1;
	int* input2;

	if(argc != 5) {
		print_usage(argv[0]);
	}

	input_base = string_to_integer(argv[1]);

	if(!valid_base(input_base)) {
		fprintf(stderr, "Invalid base: %s\n", argv[1]);
		print_usage(argv[0]);
	}

	if(!valid_input(argv[2], input_base)) {
		fprintf(stderr, "Invalid input1: %s\n", argv[2]);
		print_usage(argv[0]);
	}

	if(!valid_input(argv[4], input_base)) {
		fprintf(stderr, "Invalid input2: %s\n", argv[4]);
		print_usage(argv[0]);
	}

	char op = argv[3][0];
	if(op != '+' && op != '*' && op != '<' && op != '>' && op != '=') {
		fprintf(stderr, "Invalid operation: %s\n", argv[3]);
		print_usage(argv[0]);
	}

	input1 = string_to_integer_array(argv[2]);
	input2 = string_to_integer_array(argv[4]);

	perform_operation(input1, input2, op, input_base);

	free(input1);
	free(input2);

	exit(0);
}
