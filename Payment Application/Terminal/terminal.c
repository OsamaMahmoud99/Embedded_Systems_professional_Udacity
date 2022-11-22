#include "../Payment System/terminal.h"

#define MAX_TRANSACTION_DATE_LEN         10


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	EN_terminalError_t  status = TERMINAL_OK;
	uint8_t tempbuffer[11]={0};

	gets(tempbuffer);
	uint8_t len = strlen(tempbuffer);

	uint8_t day = (tempbuffer[0] - '0') * 10 + (tempbuffer[0] - '0');
	uint8_t month = (tempbuffer[3] - '0') * 10 + (tempbuffer[4] - '0');

	if( (len < MAX_TRANSACTION_DATE_LEN) || (day>31) || (month>12) )
	{
		status = WRONG_DATE;
	}
	else
	{
		strcpy(termData->transactionDate, tempbuffer);
		termData->transactionDate[len] = '\0';
	}

	return status;
}

void getTransactionDateTest(void)
{
	ST_terminalData_t tempterm;
	EN_terminalError_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: getTransactionDate\n");

	uint8_t test1_t[] = "12/10/2022";         //DD / MM / YYYY
	printf("Test Case 1: %s\n", test1_t);
	printf("Input Data tranc Date: ");
	status = getTransactionDate(&tempterm);

	printf("Expected Result: Date is ok\n");

	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Date is ok\n");
	}
	else if (status == WRONG_DATE)
	{
		printf("Actual Result: Date is wrong\n");
	}

	printf("--------------------------------------\n");

	uint8_t test2_t[] = "4/10/2022";             // <10 
	printf("Test Case 2: %s\n", test2_t);
	printf("Input Data tranc Date: ");
	status = getTransactionDate(&tempterm);

	printf("Expected Result: Date is wrong\n");

	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Date is ok\n");
	}
	else if (status == WRONG_DATE)
	{
		printf("Actual Result: Date is wrong\n");
	}

	printf("--------------------------------------\n");

	uint8_t test3_t[] = "32/10/2022";       //day > 31
	printf("Test Case 3: %s\n", test3_t);
	printf("Input Data tranc Date: ");
	status = getTransactionDate(&tempterm);

	printf("Expected Result: Date is wrong\n");

	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Date is ok\n");
	}
	else if (status == WRONG_DATE)
	{
		printf("Actual Result: Date is wrong\n");
	}

	printf("--------------------------------------\n");

	uint8_t test4_t[] = "30/13/2022";       //month > 12
	printf("Test Case 4: %s\n", test4_t);
	printf("Input Data tranc Date: ");
	status = getTransactionDate(&tempterm);

	printf("Expected Result: Date is wrong\n");

	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Date is ok\n");
	}
	else if (status == WRONG_DATE)
	{
		printf("Actual Result: Date is wrong\n");
	}

	printf("--------------------------------------\n");
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	EN_terminalError_t status = EXPIRED_CARD;
	uint8_t* cardpstr = cardData->cardExpirationDate;
	uint8_t* termpstr = termData->transactionDate;
	uint8_t i, flag1 = 0, flag2 = 0, card_month = 0 , term_day = 0 , term_month = 0;
	uint16_t card_year=0 , term_year=0;

	for (i = 0; cardpstr[i]; ++i)
	{
		if (cardpstr[i] != '/')
		{
			if (flag1 == 0)
			{
				card_month = card_month * 10 + cardpstr[i] - '0';
			}
			if (flag1 == 1)
			{
				card_year = card_year * 10 + cardpstr[i] - '0';
			}
		}
		else {
			flag1 = 1;
		}
	}

	for (i = 0; termpstr[i]; ++i)
	{
		if (termpstr[i] != '/')
		{
			if (flag2 == 0)
			{
				term_day = term_day * 10 + termpstr[i] - '0';
			}
			if (flag2 == 1)
			{
				term_month = term_month * 10 + termpstr[i] - '0';
			}
			if (flag2 == 2)
			{
				term_year = term_year * 10 + termpstr[i] - '0';
			}
		}
		else {
			++flag2;
		}
	}

	card_year += 2000;

	if ( (card_year > term_year) ||
		((card_month >= term_month) && (card_year == term_year)) )
	{
		status = TERMINAL_OK;
	}

	/*printf("card_month = %d\n", card_month);
	printf("term_month = %d\n", term_month);
	printf("card_year = %d\n", card_year);
	printf("term_year = %d\n", term_year);
	*/

	return status;

}

void isCardExpriedTest(void)
{
	ST_terminalData_t tempterm;
	ST_cardData_t tempcard;
	EN_terminalError_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: isCardExpired\n");

	uint8_t test1_x[] = "11/17";              //MM / YY
	printf("Test Case 1: %s\n", test1_x);
	printf("Input Data Expiry Date: ");
    getCardExpiryDate(&tempcard);

	uint8_t test1_t[] = "12/10/2022";         //DD / MM / YYYY
	printf("Test Case 1: %s\n", test1_t);
	printf("Input Data tranc Date: ");
	getTransactionDate(&tempterm);

	printf("Expected Result: CARD is EXPIRED\n");
	status = isCardExpired(&tempcard, &tempterm);

	if (status == TERMINAL_OK)
	{
		printf("Actual Result: CARD is NOT EXPIRED\n");
	}
	else if (status == EXPIRED_CARD)
	{
		printf("Actual Result: CARD is EXPIRED\n");
	}

	printf("--------------------------------------\n");

	uint8_t test2_x[] = "10/22";
	printf("Test Case 2: %s\n", test2_x);
	printf("Input Data Expiry Date: ");
	getCardExpiryDate(&tempcard);

	uint8_t test2_t[] = "12/10/2022";
	printf("Test Case 2: %s\n", test2_t);
	printf("Input Data tranc Date: ");
	getTransactionDate(&tempterm);

	printf("Expected Result: CARD is NOT EXPIRED\n");
	status = isCardExpired(&tempcard, &tempterm);

	if (status == TERMINAL_OK)
	{
		printf("Actual Result: CARD is NOT EXPIRED\n");
	}
	else if (status == EXPIRED_CARD)
	{
		printf("Actual Result: CARD is EXPIRED\n");
	}

	printf("--------------------------------------\n");

	uint8_t test3_x[] = "04/22";
	printf("Test Case 3: %s\n", test3_x);
	printf("Input Data Expiry Date: ");
	getCardExpiryDate(&tempcard);

	uint8_t test3_t[] = "12/10/2022";
	printf("Test Case 3: %s\n", test3_t);
	printf("Input Data tranc Date: ");
	getTransactionDate(&tempterm);

	printf("Expected Result: CARD is EXPIRED\n");
	status = isCardExpired(&tempcard, &tempterm);

	if (status == TERMINAL_OK)
	{
		printf("Actual Result: CARD is NOT EXPIRED\n");
	}
	else if (status == EXPIRED_CARD)
	{
		printf("Actual Result: CARD is EXPIRED\n");
	}

	printf("--------------------------------------\n");

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t status = TERMINAL_OK;

	scanf("%f", &(termData->transAmount));
	getchar();

	if (termData->transAmount <= 0) {
		status = INVALID_AMOUNT;
	}
	return status;
}
void getTransactionAmountTest(void)
{
	ST_terminalData_t tempterm;
	EN_terminalError_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: getTransactionAmount\n");

	float test1 = -5000;
	printf("Test Case 1: %0.2f\n", test1);
	printf("Input Data: ");
	
	status = getTransactionAmount(&tempterm);

	printf("Expected Result: Amount is wrong\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Amount is OK\n");
	}
	else if (status == INVALID_AMOUNT)
	{
		printf("Actual Result: Amount is wrong\n");
	}

	printf("--------------------------------------\n");

	float test2 = 0;
	printf("Test Case 2: %0.2f\n", test2);
	printf("Input Data: ");

	status = getTransactionAmount(&tempterm);

	printf("Expected Result: Amount is wrong\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Amount is OK\n");
	}
	else if (status == INVALID_AMOUNT)
	{
		printf("Actual Result: Amount is wrong\n");
	}

	printf("--------------------------------------\n");

	float test3 = 5000;
	printf("Test Case 3: %0.2f\n", test3);
	printf("Input Data: ");

	status = getTransactionAmount(&tempterm);

	printf("Expected Result: Amount is OK\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Amount is OK\n");
	}
	else if (status == INVALID_AMOUNT)
	{
		printf("Actual Result: Amount is wrong\n");
	}

	printf("--------------------------------------\n");

	float test4 = 50000;
	printf("Test Case 4: %0.2f\n", test4);
	printf("Input Data: ");

	status = getTransactionAmount(&tempterm);

	printf("Expected Result: Amount is OK\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Amount is OK\n");
	}
	else if (status == INVALID_AMOUNT)
	{
		printf("Actual Result: Amount is wrong\n");
	}

	printf("--------------------------------------\n");
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t status = TERMINAL_OK;

	if (termData->transAmount > termData->maxTransAmount)
	{
		status = EXCEED_MAX_AMOUNT;
	}
	return status;
}
void isBelowMaxAmountTest(void)
{
	ST_terminalData_t tempterm;
	EN_terminalError_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: isBelowMaxAmount\n");

	setMaxAmount(&tempterm, 50000);

	float test1 = 5000;
	printf("Test Case 1: %0.2f\n", test1);
	printf("Input Data: ");
	getTransactionAmount(&tempterm);
	status = isBelowMaxAmount(&tempterm);

	printf("Expected Result: Below MaxAmount\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Below MaxAmount\n");
	}
	else if (status == EXCEED_MAX_AMOUNT)
	{
		printf("Actual Result: EXCEED MaxAmount\n");
	}
	printf("--------------------------------------\n");


	float test2 = 50000;
	printf("Test Case 2: %0.2f\n", test2);
	printf("Input Data: ");
	getTransactionAmount(&tempterm);
	status = isBelowMaxAmount(&tempterm);

	printf("Expected Result: Below MaxAmount\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Below MaxAmount\n");
	}
	else if (status == EXCEED_MAX_AMOUNT)
	{
		printf("Actual Result: EXCEED MaxAmount\n");
	}
	printf("--------------------------------------\n");


	float test3 = 60000;
	printf("Test Case 3: %0.2f\n", test3);
	printf("Input Data: ");
	getTransactionAmount(&tempterm);
	status = isBelowMaxAmount(&tempterm);

	printf("Expected Result: EXCEED MaxAmount\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: Below MaxAmount\n");
	}
	else if (status == EXCEED_MAX_AMOUNT)
	{
		printf("Actual Result: EXCEED MaxAmount\n");
	}
	printf("--------------------------------------\n");

}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	EN_terminalError_t status = TERMINAL_OK;

	if (maxAmount <= 0)
	{
		status = INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = maxAmount;
	}
	
	return status;
}
void setMaxAmountTest(void)
{
	ST_terminalData_t tempterm;
	EN_terminalError_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: setMaxAmount\n");

	float test1 = -5000;
	printf("Test Case 1: %0.2f\n", test1);

	status = setMaxAmount(&tempterm , test1);

	printf("Expected Result: MaxAmount is wrong\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: MaxAmount is OK\n");
	}
	else if (status == INVALID_MAX_AMOUNT)
	{
		printf("Actual Result: MaxAmount is wrong\n");
	}

	printf("--------------------------------------\n");

	float test2 = 0;
	printf("Test Case 2: %0.2f\n", test2);

	status = setMaxAmount(&tempterm,test2);

	printf("Expected Result: MaxAmount is wrong\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: MaxAmount is OK\n");
	}
	else if (status == INVALID_MAX_AMOUNT)
	{
		printf("Actual Result: MaxAmount is wrong\n");
	}

	printf("--------------------------------------\n");

	float test3 = 5000;
	printf("Test Case 3: %0.2f\n", test3);

	status = setMaxAmount(&tempterm,test3);

	printf("Expected Result: MaxAmount is OK\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: MaxAmount is OK\n");
	}
	else if (status == INVALID_MAX_AMOUNT)
	{
		printf("Actual Result: MaxAmount is wrong\n");
	}

	printf("--------------------------------------\n");

	float test4 = 50000;
	printf("Test Case 4: %0.2f\n", test4);

	status = setMaxAmount(&tempterm,test4);

	printf("Expected Result: MaxAmount is OK\n");
	if (status == TERMINAL_OK)
	{
		printf("Actual Result: MaxAmount is OK\n");
	}
	else if (status == INVALID_MAX_AMOUNT)
	{
		printf("Actual Result: MaxAmount is wrong\n");
	}

	printf("--------------------------------------\n");
}

