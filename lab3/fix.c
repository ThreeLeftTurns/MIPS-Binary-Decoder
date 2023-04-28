// Michael Cali 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LENGTH 32
#define false 0
#define true 1

//check function to ensure binary is 32 bits long
int check(char* bits) {
    if (strlen(bits) != LENGTH) {
        return false;
    }
    for (int i = 0; i < LENGTH; i++) {
        if (bits[i] != '0' && bits[i] != '1') {
        return false;
        }
    }
    return true;
}

// Parse the char array (of '0' and '1' characters) into its
// unsigned binary equivalent.
unsigned parse_bits(char *bits)
{
	unsigned result = 0;
	int i;
	int pos = 0;
	for (i=strlen(bits)-1; i >= 0; i--) {
		if (bits[i] == '1') {
			result |= 1 << pos;
		}
		pos++;
	}
	return result;
}


//Register Table Lookup
char* RegisterComp(char* Reg1)
{
    typedef struct
    {
    char *name;
    char *binary;
    } registers; 

    registers registerTable[] =
    {
        {"$0", "00000"},
        {"$at", "00001"},
        {"$v0", "00010"},
        {"$v1", "00011"},
        {"$a0", "00100"},
        {"$a1", "00101"},
        {"$a2", "00110"},
        {"$a3", "00111"},
        {"$t0", "01000"},
        {"$t1", "01001"},
        {"$t2", "01010"},
        {"$t3", "01011"},
        {"$t4", "01100"},
        {"$t5", "01101"},
        {"$t6", "01110"},
        {"$t7", "01111"},
        {"$s0", "10000"},
        {"$s1", "10001"},
        {"$s2", "10010"},
        {"$s3", "10011"},
        {"$s4", "10100"},
        {"$s5", "10101"},
        {"$s6", "10110"},
        {"$s7", "10111"},
        {"$t8", "11000"},
        {"$t9", "11001"},
        {"$k0", "11010"},
        {"$k1", "11011"},
        {"$gp", "11100"},
        {"$sp", "11101"},
        {"$fp", "11110"},
        {"$ra", "11111"}
    };

        for(int i = 0; i < 32; i++ )
        {
            if(strcmp(Reg1 , registerTable[i].binary) == 0)
            {
            return registerTable[i].name;
            }
        }
}

//R_type instruction lookup
char* R_type(char* funct)
{
    
    typedef struct{
        char *name;
        char *binary;
    } registers; 

    registers operations[] = 
    {
        {"add", "100000"},
        {"sub", "100010"},
        {"slt", "101010"},
        {"sll", "000000"},
        {"srl", "000010"}
    };
    
    for(int i = 0; i < 5; i++ )
    {
        if(strcmp(funct , operations[i].binary) == 0)
        {
            return operations[i].name;
        }
    }
    
}

int main(int argc, char* argv[])
{
   // Get a pointer to the argument.
    char *instruction = argv[1];

    if (!check(instruction)) {
        fprintf(stderr, "Invalid bit string\n");
        return -2;
    }
    //creates the variables to hold the each section of binary
    char opcode[32];
    char rs[32];
    char rt[32];
    char rd[32]; 
    char shamt[32];
    char funct[32];
    char value[32];
    char jumptohex[32];

    //for r types
    char Rtypes[] = {"000000"};

    //for i types
    char addi[] = {"001000"};
    char lw[] = {"100011"};
    char sw[] = {"101011"};
    char beq[] = {"000100"};

    //for j-types
    char j[] = {"000010"};
    char jr[] = {"000011"};

    if(argc > 1)
    {
        char *binaryVal= argv[1];

        strncpy(opcode, binaryVal, 6);
        opcode[6] = '\0';
        strncpy(rs, binaryVal + 6, 5);
        rs[5] = '\0';
        strncpy(rt, binaryVal + 11, 5);
        rt[5] = '\0';
        strncpy(rd, binaryVal + 16, 5);
        rd[5] = '\0';
        strncpy(shamt, binaryVal + 21, 5);
        shamt[5] = '\0';
        strncpy(funct, binaryVal + 26, 6);
        funct[6] = '\0';

        //for "i" instructions
        strncpy(value, binaryVal + 16, 16);
        value[16] = '\0';

        //for "j" instructions
        strncpy(jumptohex, binaryVal+8, 22);
        jumptohex[22] = '\0';

    }
    
    char* values = value;
    // printf("%s\n", opcode);
    // printf("%s\n", rs);
    // printf("%s\n", rt);
    // printf("%s\n", rd);
    // printf("%s\n", shamt);
    // printf("%s\n", funct);
    
    unsigned result = parse_bits(values);

    unsigned shamtconv = parse_bits(shamt); 

    unsigned hexconv = parse_bits(jumptohex);
    
    unsigned functconv = parse_bits(funct);

    if((strncmp(opcode, Rtypes, 6) == 0))
    {        
        if(strncmp(rs,"11111",5) == 0)
        {
            printf("jr $ra");
        }
        

        char *roperation = R_type(funct);
        char *register1 = RegisterComp(rs); 
        char *destination = RegisterComp(rt);
        char *register2 = RegisterComp(rd); 


          //for SRL and SLL 
        if(roperation == "srl" || roperation == "sll")
        {
            printf("%s", roperation);
            printf(" %s,",register2);
            printf(" %s, ",destination);
            printf("%d", shamtconv);
            return 1;
        }

        printf("%s", roperation);
       
        printf(" %s,",register2);
        
        printf(" %s,",register1);
        
        printf(" %s",destination);

      

      

    }
    else if (strncmp(opcode, addi, 6)== 0) 
    {
        printf("addi ");
        char *register1 = RegisterComp(rs); 
        char *destination = RegisterComp(rt);
       
        printf("%s,",destination);
        printf(" %s,",register1);
        printf(" %d", result);
        
    }
    else if (strncmp(opcode, lw, 6)== 0) 
    {
        printf("lw ");
        char *register1 = RegisterComp(rs); 
        char *destination = RegisterComp(rt);

        printf(" %s,",destination);
        printf(" %d",result);
        printf("(%s)", register1);
    }
    else if(strncmp(opcode, sw, 6)== 0)
    {
        printf("sw ");
        char *register1 = RegisterComp(rs); 
        char *destination = RegisterComp(rt);

        printf(" %s,",destination);
        printf(" %d",result);
        printf("(%s)", register1);


    }
    else if(strncmp(opcode, beq, 6)== 0)
    {
        printf("beq");
        char *register1 = RegisterComp(rs); 
        char *destination = RegisterComp(rt);

        printf(" %s,",register1);
        printf(" %s, ", destination);
        printf("%X", value);


    }
     else if(strncmp(opcode, j, 6)== 0)
    {
        printf("j ");
        printf("%X", jumptohex);
    } 

    else
    {
        printf("Not here");
    }
}

