#include "card.h"


#define MAX_HOLDER_NAME_LEN      24
#define MIN_HOLDER_NAME_LEN      20

#define MAX_EXPIRY_DATE_LEN      5

#define MAX_ACCOUNT_NUM_LEN      19
#define MIN_ACCOUNT_NUM_LEN      16

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	EN_cardError_t returnval = OK;
	uint8_t tempbuffer[25]={0};

	//This function will ask for the cardholder's name.
	gets(tempbuffer);

	uint8_t namelen;
	namelen = strlen(tempbuffer);

	if ((namelen< MIN_HOLDER_NAME_LEN) || (namelen> MAX_HOLDER_NAME_LEN) )
	{
		returnval = WRONG_NAME;
	}
	else
	{
		//store name into card data.
		strcpy(cardData->cardHolderName, tempbuffer);
		cardData->cardHolderName[namelen] = '\0';
	}

	return returnval;
}

void getCardHolderNameTest(void)
{
	ST_cardData_t tempcard;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: getCardHolderName\n");

	uint8_t test1[] = "Osama Mahmoud Hanafy";
	printf("Test Case 1: %s\n", test1);
	printf("Input Data: ");
	EN_cardError_t returnval = getCardHolderName(&tempcard);
	printf("Expected Result: Holdername is OK\n");
	if (returnval == OK)
	{
		printf("Actual Result: Holdername is OK\n");
	}
	else if (returnval == WRONG_NAME)
	{
		printf("Actual Result: Holdername is WRONG\n");
	}
	printf("--------------------------------------\n");

	uint8_t test2[] = "Osama Mahmoud Hanafy Ahmed";
	printf("Test Case 2: %s\n", test2);
	printf("Input Data: ");
	returnval = getCardHolderName(&tempcard);
	printf("Expected Result: Holdername is WRONG\n");
	if (returnval == OK)
	{
		printf("Actual Result: Holdername is OK\n");
	}
	else if (returnval == WRONG_NAME)
	{
		printf("Actual Result: Holdername is WRONG\n");
	}
	printf("--------------------------------------\n");

	uint8_t test3[] = "Osama Mahmoud";
	printf("Test Case 3: %s\n", test3);
	printf("Input Data: ");
	returnval = getCardHolderName(&tempcard);
	printf("Expected Result: Holdername is WRONG\n");
	if (returnval == OK)
	{
		printf("Actual Result: Holdername is OK\n");
	}
	else if (returnval == WRONG_NAME)
	{
		printf("Actual Result: Holdername is WRONG\n");
	}
	printf("--------------------------------------\n");
}
/************************************************************************************/

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	EN_cardError_t returnval = OK;
	uint8_t tempbuffer[7] = { 0 };

	//This function will ask for the cardholder's name.
	gets(tempbuffer);

	uint8_t Datelen;
	Datelen = strlen(tempbuffer);


	if ( (Datelen < MAX_EXPIRY_DATE_LEN) || (Datelen > MAX_EXPIRY_DATE_LEN) || (tempbuffer[2]!= '/')
		 || (tempbuffer[0] >= '1' && tempbuffer[1] > '2') )
	{
		returnval = WRONG_EXP_DATE;
	}
	else if (!(tempbuffer[0] >= '0' && tempbuffer[0] <= '9') && !(tempbuffer[1] >= '0' && tempbuffer[1] <= '9')
		&& !(tempbuffer[3] >= '0' && tempbuffer[3] <= '9') && !(tempbuffer[4] >= '0' && tempbuffer[4] <= '9'))
	{
		returnval = WRONG_EXP_DATE;
	}
	else
	{
		//store Date into card data.
		strcpy(cardData->cardExpirationDate, tempbuffer);
		cardData->cardExpirationDate[Datelen] = '\0';
	}
	return returnval;
}

void getCardExpiryDateTest(void)
{

	ST_cardData_t tempcard;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: getCardExpiryDate\n");


	uint8_t test1[] = "05/25"; 
	printf("Test Case 1: %s\n", test1);
	printf("Input Data: ");
	EN_cardError_t returnval = getCardExpiryDate(&tempcard);
	printf("Expected Result: CARD DATE is OK\n");
	if (returnval == OK)
	{
		printf("Actual Result: CARD DATE is OK\n");
	}
	else if (returnval == WRONG_EXP_DATE)
	{
		printf("Actual Result: CARD DATE is WRONG\n");
	}

	printf("--------------------------------------\n");
	uint8_t test2[] = "17/25";                  
	printf("Test Case 2: %s\n", test2);
	printf("Input Data: ");
	returnval = getCardExpiryDate(&tempcard);
	printf("Expected Result: CARD DATE is WRONG\n");
	if (returnval == OK)
	{
		printf("Actual Result: CARD DATE is OK\n");
	}
	else if (returnval == WRONG_EXP_DATE)
	{
		printf("Actual Result: CARD DATE is WRONG\n");
	}

	printf("--------------------------------------\n");
	uint8_t test3[] = "12/5";            
	printf("Test Case 3: %s\n", test3);
	printf("Input Data: ");
	returnval = getCardExpiryDate(&tempcard);
	printf("Expected Result: CARD DATE is WRONG\n");
	if (returnval == OK)
	{
		printf("Actual Result: CARD DATE is OK\n");
	}
	else if (returnval == WRONG_EXP_DATE)
	{
		printf("Actual Result: CARD DATE is WRONG\n");
	}
	printf("--------------------------------------\n");

	uint8_t test4[] = "12/255";
	printf("Test Case 4: %s\n", test4);
	printf("Input Data: ");
	returnval = getCardExpiryDate(&tempcard);
	printf("Expected Result: CARD DATE is WRONG\n");
	if (returnval == OK)
	{
		printf("Actual Result: CARD DATE is OK\n");
	}
	else if (returnval == WRONG_EXP_DATE)
	{
		printf("Actual Result: CARD DATE is WRONG\n");
	}
	printf("--------------------------------------\n");
}

/************************************************************************************/

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	EN_cardError_t returnval = OK;
	uint8_t tempbuffer[20] = { 0 };

	//This function will ask for the cardholder's name.
	gets(tempbuffer);

	uint8_t Numlen;
	Numlen = strlen(tempbuffer);

	if ((Numlen < MIN_ACCOUNT_NUM_LEN) || (Numlen > MAX_ACCOUNT_NUM_LEN))
	{
		returnval = WRONG_PAN;
	}
	else
	{
		//store name into card data.
		strcpy(cardData->primaryAccountNumber, tempbuffer);
		cardData->primaryAccountNumber[Numlen] = '\0';
	}

	return returnval;
}

void getCardPANTest(void)
{
	
	ST_cardData_t tempcard;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: getCardPAN\n");

	
	uint8_t test1[] = "1234567891234567891";
	printf("Test Case 1: %s\n", test1);
	printf("Input Data: ");
	EN_cardError_t returnval = getCardPAN(&tempcard);
	printf("Expected Result: CARDPAN is OK\n");
	if (returnval == OK)
	{
		printf("Actual Result: CARDPAN is OK\n");
	}
	else if (returnval == WRONG_PAN)
	{
		printf("Actual Result: CARDPAN is WRONG\n");
	}

	printf("--------------------------------------\n");
	uint8_t test2[] = "123456789101123";                  // less than 16 char
	printf("Test Case 2: %s\n", test2);
	printf("Input Data: ");
	returnval = getCardPAN(&tempcard);
	printf("Expected Result: CARDPAN is WRONG\n");
	if (returnval == OK)
	{
		printf("Actual Result: CARDPAN is OK\n");
	}
	else if (returnval == WRONG_PAN)
	{
		printf("Actual Result: CARDPAN is WRONG\n");
	}
	
	printf("--------------------------------------\n");
	uint8_t test3[] = "123456789101112131415";            // more than 19 char
	printf("Test Case 3: %s\n", test3);
	printf("Input Data: ");
	returnval = getCardPAN(&tempcard);
	printf("Expected Result: CARDPAN is WRONG\n");
	if (returnval == OK)
	{
		printf("Actual Result: CARDPAN is OK\n");
	}
	else if (returnval == WRONG_PAN)
	{
		printf("Actual Result: CARDPAN is WRONG\n");
	}
	printf("--------------------------------------\n");
}