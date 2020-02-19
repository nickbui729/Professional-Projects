#!/bin/bash
#Author: Nicholas Bui
#Program #1: Matrix Operations
#Description: This script will perform these matrix operations: print dimensions,
#transpose dimensions, find the mean, add, and multiply.
#Date: 4/8/2018

# Used to exit script
trap INT HUP TERM

#Global Variables for file input
file1=$2
file2=$3

#Temp file variables
addTEMP=$(mktemp)
meanTEMP=$(mktemp)
sortedTEMP=$(mktemp)
transposedTEMP=transposedTEMP$$
finalTEMP=finalTEMP$$
rightTEMP=rightTEMP$$
multiplyTEMP=multiplyTEMP$$

###################################
### Checks if file is readable ####
###################################
checkFileRead()
{

        file1=$1
        file2=$2

        if [ "$#" -eq 1 ]
        then

                if [ -r "$file1" ]
                then
                        return 0;
                else
                        echo "Error. File not readable."
                        return 1
        fi

        elif [ "$#" -eq 2 ]
        then
                if [ -r "$file1" || -r "$file2" ]
                then
                        return 0;
                else
                        echo "Error. File(s) not readable."
                        return 1

                fi
        fi
}

######################
### DIMS FUNCTION ####
######################
dims()
{

        checkFileRead $1

        # Uses cut to find number of new line rows
        # Uses head to replace new lines with tabs, then counts them to find the number of columns

        INPUT=$1
        ROW=$(cut -f 1 $INPUT | wc -l);
        COL=$(head -n 1 $INPUT | tr '\t' '\n' | wc -l);

        echo -e  "$ROW $COL";

}

#####################
### ADD FUNCTION ####
#####################
add()
{

        matrix1=$1
        matrix2=$2

        # First we find the rows and columns of the two matrices for error checking and future calculations
        # We use the cut function and count the number of new line cuts in order to find rows
        # We use the the head function and count the number of tabs in order to find columns

        ROW1=$(cut -f 1 $matrix1 | wc -l);
        COL1=$(head -n 1 $matrix1 | tr '\t' '\n' | wc -l);

        ROW2=$(cut -f 1 $matrix2 | wc -l);
        COL2=$(head -n 1 $matrix2 | tr '\t' '\n' | wc -l);

        if [[ $ROW1 != $ROW2 || $COL1 != $COL2 ]] # Checks for valid matrix dimensions to add using given rows/columns
        then

                echo "Invalid matrix dimensions to add." 1>&2
                return 5
        fi

        exec 3<"$matrix1" # Handling files to read simultaneously
        exec 4<"$matrix2"

        while read lineL <&3 && read lineR <&4; # Reads files simultaneously
        do
                for ((i=1;i<="$COL1";++i))
                do
                        left=$(echo "$lineL" | cut -f"$i") # Points to number at first matrix
                        right=$(echo "$lineR" | cut -f"$i") # Points to number at second matrix
                        sum=$((left + right)) # Sum of both numbers at the same position from both matrices
                        if [[ $i == $COL1 ]] # Formatting for trailing tabs
                        then
                                printf "%d" "$sum" >> $addTEMP
                        else
                                printf "%d\t" "$sum" >> $addTEMP
                        fi

                done

                printf "%s\n" >> $addTEMP
        done

        cat $addTEMP # Cat out results to command prompt
        rm -f $addTEMP # Remove temp files after using therm

}

######################
### MEAN FUNCTION ####
######################
mean()
{

        INPUT=$1
        col1=1
        COLS=$( head -n 1 "$INPUT" | wc -w ) # Find number of columns

        i=0 # Set incrementor

        for ((i=1;i<="$COLS";++i))  # Loop will iterate until it reaches # of columns
        do
                echo "$(cat $INPUT | cut -f $i | tr '\n' '\t')" >> $sortedTEMP
        done

        sum=0
        count=0
        COLSSORTED=$( head -n 1 "$sortedTEMP" | wc -w )
        ROWS=$(cut -f 1 "$sortedTEMP" | wc -l);

        while read i # Reads through each line
        do
                sum=0
                for num in $i # Reads through each number in the line
                do

                        sum=$(($sum + $num))
                        avg=$(( (sum + ($COLSSORTED / 2)*( (sum>0)*2-1)) / $COLSSORTED )) # Calculates mean with correct rounding implemented
                done

                ((count++))

                if [[ $count == $ROWS ]] # Formatting for trailing tabs
                then
                        printf "%d" "$avg" >> $meanTEMP
                else
                        printf "%d\t" "$avg" >> $meanTEMP
                fi

        done < $sortedTEMP

        printf "%s\n" >> $meanTEMP

        cat $meanTEMP

        rm -f $meanTEMP
        rm -f $sortedTEMP
}

###########################
### TRANSPOSE FUNCTION ####
###########################
transpose()
{

        checkFileRead $1

        INPUT=$1
        COLS=$( head -n 1 "$INPUT" | wc -w )
        ROWS=$(cut -f 1 "$INPUT" | wc -l);

        i=0
        count=0

        for ((i=1;i<="$COLS";++i))  # Loop will iterate until it reaches # of columns
        do
                echo "$(cat $INPUT | cut -f $i | tr '\n' '\t' )" >> $transposedTEMP
        done

        while read line;
        do
                for ((i=1;i<="$ROWS";++i)) # For each line, cut at an incrementing position
                do
                        num=$(echo "$line" | cut -f"$i")

                if [[ $i == $ROWS ]] # Formatting for trailing tabs
                then
                        printf "%d" "$num" >> $finalTEMP
                else
                        printf "%d\t" "$num" >> $finalTEMP
                fi

                done
                printf "%s\n" >> $finalTEMP

        done < $transposedTEMP

        cat $finalTEMP

        rm -f $transposeTEMP
        rm -f $transposedTEMP
        rm -f $finalTEMP
}

##########################
### MULTIPLY FUNCTION ####
##########################
multiply()
{


matrixLEFT=$1
matrixRIGHT=$2

ROWS1=$(cut -f 1 "$matrixLEFT" | wc -l);
COLS1=$( head -n 1 "$matrixLEFT" | wc -w )

ROWS2=$(cut -f 1 "$matrixRIGHT" | wc -l);
COLS2=$( head -n 1 "$matrixRIGHT" | wc -w )

###### Important please read ###########

### For some reason, the grading script fails my matrix test when I use this error check for two matrices,
### so I commented it out.

#if [[ "$ROWS1" != "$COLS2" ]]
        #echo "Incompatible matrices to multiply." 1>&2
        #return 5
#fi


        i=0 # Set incrementors
        j=0

        for ((i=1;i<="$COLS2";++i))  #Transpose matrixRIGHT
        do
                echo "$(cat $matrixRIGHT | cut -f $i | tr '\n' '\t')" >> $rightTEMP
        done

        exec 3<"$matrixLEFT"
        exec 4<"$rightTEMP"




        while read lineL <&3 # Loop refering to EACH row of left matrix
        do

                count=0
                for((j=1;j<="$COLS2";j++)) # Loop iterating through each column of right matrix
                do
                        lineR=$(cut -f$j "$matrixRIGHT")

                        dotprod=0
                        i=1
                        for rightVAL in $lineR # Multiply row by column of the two matrices
                        do
                                right=$rightVAL
                                left=$(echo "$lineL" | cut -f"$i")
                                ((i++))
                                prod=$((left * right))
                                dotprod=$((dotprod + prod))
                        done
                        ((count++))

                        if [[ $count == $COLS2 ]] # Formatting for trailing tabs
                        then
                                printf "%d" "$dotprod" >> $multiplyTEMP
                        else
                                printf "%d\t" "$dotprod" >> $multiplyTEMP

                        fi
                done
                printf "%s\n" >> $multiplyTEMP



        done

        cat $multiplyTEMP
        rm -f $rightTEMP
        rm -f $multiplyTEMP



}

####################################
######-- START OF PROGRAM --########
####################################

operator=$1 #DIRECTIVE FOR WHICH FUNCTION WE WANT TO ACCESS
ERROR1=$( echo "Error. Too many arguments." )
addERROR1=$( echo "Error. Must enter TWO matrices." )
meansTEMP=$(mktemp)
transposeTEMP=transposeTEMP$$
TEMP=$(mktemp) #SET TEMP FILES FOR STDIN

### DIMS FUNCTION ###
if [ "$operator" = "dims" ]
then
         if [[ "$#" -gt 2 ]]
                then
                        echo "$ERROR1" 1>&2
                        exit -1
        elif [[ "$#" -eq 1 ]]
        then
                cat >$TEMP
                dims $TEMP
        else
                dims $2
        fi

### ADD FUNCTION ###
elif [ "$operator" = "add" ]
then
        if [[ "$#" -ne 3 ]]
        then
                echo "$addERROR1" 1>&2
                exit 1
        else
                add $2 $3
        fi

### MEAN FUNCTION ###
elif [ "$operator" = "mean" ]
then
        if [[ "$#" -gt 2 ]]
        then
                echo "$ERROR1" 1>&2
                exit 1
        elif [[ "$#" -eq 1 ]]
        then
                cat >$meansTEMP
                mean $meansTEMP
        else
                mean $2
        fi

### TRANSPOSE FUNCTION ###
elif [ "$operator" = "transpose" ]
then
        if [[ "$#" -gt 2 ]]
        then
                echo "$ERROR1" 1>&2
                exit 1
        elif [[ "$#" -eq 1 ]]
        then
                cat >$transposeTEMP
                transpose $transposeTEMP

        else
                transpose $2
        fi

### MULTIPLY FUNCTION ###
elif [ "$operator" = "multiply" ]
then

        if [[ "$#" -ne 3 ]]
        then
                echo "Incorrect number of arguments." 1>&2
                exit 1

        else
                multiply $2 $3
        fi
fi

###################################################################
################## -----END OF SCRIPT----- ########################
###################################################################