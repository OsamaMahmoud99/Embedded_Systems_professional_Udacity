#include "../Payment System/server.h"

#define MAX_ACCOUNT_DB          255

ST_accountsDB_t accountsDB[MAX_ACCOUNT_DB] =
{
	{12000.0, RUNNING, "5594454329578106"},          //Transaction approved user story
	{15000.0, RUNNING, "5807007076043875"},         //Exceed the maximum amount user story
	{8000.0, RUNNING, "6912018075412687"},        //Insufficient fund user story
    {5000.0, RUNNING, "1256891063451456"},       //Expired card user story
	{12000.0, BLOCKED, "5594454311578106"}       //Stolen card user story
};

ST_transaction_t transactionsDB[255] = { 0 };

static uint8_t account_ref;
static uint32_t sequencenumber = 1000;

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_serverError_t isvalidaccount , isvalidamount , isblockedaccount , issavedtrans;

	transData->transState = APPROVED;

	//printf("Enter Card Holder Data\n");
	isvalidaccount = isValidAccount(&(transData->cardHolderData), accountsDB);

	//printf("Enter the transaction Amount: ");
	isvalidamount = isAmountAvailable(&(transData->terminalData), accountsDB);

	isblockedaccount = isBlockedAccount(accountsDB);

	if (isvalidaccount == ACCOUNT_NOT_FOUND) {
		transData->transState = FRAUD_CARD;
		return transData->transState;
	}
	else if (isvalidamount == LOW_BALANCE) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return transData->transState;
	}
	else if (isblockedaccount == BLOCKED_ACCOUNT) {
		transData->transState = DECLINED_STOLEN_CARD;
		return transData->transState;
	}

	transData->transactionSequenceNumber = sequencenumber;

	issavedtrans = saveTransaction(transData);
	if (issavedtrans == SAVING_FAILED) {
		transData->transState = INTERNAL_SERVER_ERROR;
		return transData->transState;
	}

	if ((isBelowMaxAmount(&(transData->terminalData)) == TERMINAL_OK) && (isvalidaccount == SERVER_OK)){

		accountsDB[account_ref].balance -= transData->terminalData.transAmount;
	}

	return transData->transState;
	
}

void recieveTransactionDataTest(void)
{
	ST_transaction_t temptransaction;
	EN_transState_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: recieveTransactionData\n");

	account_ref = 0;
	temptransaction.terminalData.transAmount = 5000;
	temptransaction.terminalData.maxTransAmount = 10000;

	uint8_t test1_n[] = "Osama Mahmoud Hanafy", test1_d[] = "05/25", test1_p[] = "5594454329578106";
	printf("Test Case 1: Name:%s Date:%s PAN:%s\n", test1_n, test1_d, test1_p);
	printf("Test Case 1: 5000 from 12000\n");
	printf("Test Case 1: RUNNING\n");
	printf("Input Data: \n");
	status = recieveTransactionData(&temptransaction);
	printf("Expected Result: Account Found\n");
	if (status == FRAUD_CARD)
	{
		printf("Actual Result: Account Not Found\n");
	}
	else if (status == APPROVED)
	{
		printf("Actual Result: Account Found\n");
	}
	printf("--------------------------------------\n");	

	account_ref = 1;
	temptransaction.terminalData.transAmount = 5000;
	temptransaction.terminalData.maxTransAmount = 10000;

	uint8_t test2_n[] = "Osama Mahmoud", test2_d[] = "05/25", test2_p[] = "5807007076043875";
	printf("Test Case 2: Name:%s Date:%s PAN:%s\n", test2_n, test2_d, test2_p);
	printf("Test Case 2: 5000 from 15000\n");
	printf("Test Case 2: RUNNING\n");
	printf("Input Data: \n");
	status = recieveTransactionData(&temptransaction);
	printf("Expected Result: Account Not Found\n");
	if (status == FRAUD_CARD)
	{
		printf("Actual Result: Account Not Found\n");
	}
	else if (status == APPROVED)
	{
		printf("Actual Result: Account Found\n");
	}
	printf("--------------------------------------\n");

	account_ref = 4;
	temptransaction.terminalData.transAmount = 5000;
	temptransaction.terminalData.maxTransAmount = 10000;

	uint8_t test3_n[] = "Osama Mahmoud Hanafy", test3_d[] = "05/25", test3_p[] = "5594454311578106";
	printf("Test Case 3: Name:%s Date:%s PAN:%s\n", test3_n, test3_d, test3_p);
	printf("Test Case 3: 5000 from 12000\n");
	printf("Test Case 3: Blocked\n");
	printf("Input Data: \n");
	status = recieveTransactionData(&temptransaction);
	printf("Expected Result: Account is Blocked\n");
	if (status == DECLINED_STOLEN_CARD)
	{
		printf("Actual Result: Account is Blocked\n");
	}
	else if (status == APPROVED)
	{
		printf("Actual Result: Account is Running\n");
	}
	printf("--------------------------------------\n");

	account_ref = 2;
	temptransaction.terminalData.transAmount = 9000;
	temptransaction.terminalData.maxTransAmount = 10000;

	uint8_t test4_n[] = "Osama Mahmoud Hanafy", test4_d[] = "05/25", test4_p[] = "6912018075412687";
	printf("Test Case 4: Name:%s Date:%s PAN:%s\n", test4_n, test4_d, test4_p);
	printf("Test Case 4: 9000 from 8000\n");
	printf("Test Case 4: RUNNING\n");
	printf("Input Data: \n");
	status = recieveTransactionData(&temptransaction);
	printf("Expected Result: Low Balance\n");
	if (status == DECLINED_INSUFFECIENT_FUND)
	{
		printf("Actual Result: Low Balance\n");
	}
	else if (status == APPROVED)
	{
		printf("Actual Result: Amount is available\n");
	}
	printf("--------------------------------------\n");

	account_ref = 3;
	temptransaction.terminalData.transAmount = 3000;
	temptransaction.terminalData.maxTransAmount = 10000;
	sequencenumber = 1500;

	uint8_t test5_n[] = "Osama Mahmoud Hanafy", test5_d[] = "05/25", test5_p[] = "1256891063451456";
	printf("Test Case 5: Name:%s Date:%s PAN:%s\n", test5_n, test5_d, test5_p);
	printf("Test Case 5: 3000 from 5000\n");
	printf("Test Case 5: RUNNING\n");
	printf("Input Data: \n");
	status = recieveTransactionData(&temptransaction);
	printf("Expected Result: INTERNAL_SERVER_ERROR\n");
	if (status == INTERNAL_SERVER_ERROR)
	{
		printf("Actual Result: INTERNAL_SERVER_ERROR\n");
	}
	else if (status == APPROVED)
	{
		printf("Actual Result: Saving is ok\n");
	}
	printf("--------------------------------------\n");

}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	//EN_cardError_t isvalidname = OK , isvalidpan = OK , isvalidDate;
	uint8_t i = 0;

	//accountRefrence = accountsDB;

	/*printf("Enter the card Name:  ");
	isvalidname = getCardHolderName(cardData);

	printf("Enter the card PAN:  ");
	isvalidpan = getCardPAN(cardData);

	printf("Enter the card EXP Date:  ");
	isvalidDate = getCardExpiryDate(cardData);
	*/

	//if (isvalidname == OK && isvalidpan == OK && isvalidDate == OK)
	//{
		for (i = 0; i < MAX_ACCOUNT_DB; ++i)
		{
			if (strcmp(cardData->primaryAccountNumber, accountRefrence[i].primaryAccountNumber) == 0) {
				 account_ref = i;
				 return SERVER_OK;
			}
		}
	//}

	return ACCOUNT_NOT_FOUND;
}

void isValidAccountTest(void)
{
	ST_cardData_t tempcard;
	EN_serverError_t returnval;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: isValidAccount\n");

	uint8_t test1_n[] = "Osama Mahmoud Hanafy" , test1_d[]= "05/25" , test1_p[]= "5594454329578106";
	printf("Test Case 1: Name:%s Date:%s PAN:%s\n", test1_n , test1_d , test1_p);
	printf("Input Data: \n");
	returnval = isValidAccount(&tempcard , accountsDB);
	printf("Expected Result: Account Found\n");
	if (returnval == SERVER_OK)
	{
		printf("Actual Result: Account Found\n");
	}
	else if (returnval == ACCOUNT_NOT_FOUND)
	{
		printf("Actual Result: Account Not Found\n");
	}
	printf("--------------------------------------\n");

	uint8_t test2_n[] = "Osama Mahmoud Hanafy", test2_d[] = "05/25", test2_p[] = "5594456129578106";
	printf("Test Case 2: Name:%s Date:%s PAN:%s\n", test2_n, test2_d, test2_p);
	printf("Input Data: \n");
	returnval = isValidAccount(&tempcard, accountsDB);
	printf("Expected Result: Account Not Found\n");
	if (returnval == SERVER_OK)
	{
		printf("Actual Result: Account Found\n");
	}
	else if (returnval == ACCOUNT_NOT_FOUND)
	{
		printf("Actual Result: Account Not Found\n");
	}
	printf("--------------------------------------\n");
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	//accountRefrence = accountsDB;
	
	if ( accountRefrence[account_ref].state == BLOCKED){
			return BLOCKED_ACCOUNT;
	}
	if ( accountRefrence[account_ref].state == RUNNING) {
			return SERVER_OK;
	}
}

void isBlockedAccountTest(void)
{
	EN_serverError_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: isBlockedAccount\n");

	account_ref = 1;
	printf("Test Case 1: RUNNING\n");
	printf("Input Data: %d\n",account_ref);
	status = isBlockedAccount(accountsDB);
	printf("Expected Result: Account RUNNING\n");
	if (status == BLOCKED_ACCOUNT)
	{
		printf("Actual Result: Account BLOCKED\n");
	}
	else if (status == SERVER_OK)
	{
		printf("Actual Result: Account RUNNING\n");
	}
	printf("--------------------------------------\n");

	account_ref = 4;
	printf("Test Case 2: BLOCKED\n");
	printf("Input Data: %d\n", account_ref);
	status = isBlockedAccount(accountsDB);
	printf("Expected Result: Account BLOCKED\n");
	if (status == BLOCKED_ACCOUNT)
	{
		printf("Actual Result: Account BLOCKED\n");
	}
	else if (status == SERVER_OK)
	{
		printf("Actual Result: Account RUNNING\n");
	}
	printf("--------------------------------------\n");

}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	//accountRefrence = accountsDB;
	EN_terminalError_t isvalidamount = TERMINAL_OK;

	//isvalidamount = getTransactionAmount(termData);

	//if (isvalidamount == TERMINAL_OK) {

		if (termData->transAmount > accountRefrence[account_ref].balance) {
			return LOW_BALANCE;
		}

		return SERVER_OK;
	//}
}

void isAmountAvailableTest(void)
{
	EN_serverError_t status;
	ST_terminalData_t termtemp;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: isAmountAvailable\n");

	account_ref = 0;
	printf("Test Case 1: 5000 from 12000\n");
	printf("Input Data: ");
	status = isAmountAvailable(&termtemp,accountsDB);
	printf("Expected Result: Amount is available\n");
	if (status == SERVER_OK)
	{
		printf("Actual Result: Amount is available\n");
	}
	else if (status == LOW_BALANCE)
	{
		printf("Actual Result: Amount is not available\n");
	}
	printf("--------------------------------------\n");

	account_ref = 1;
	printf("Test Case 2: 16000 from 15000\n");
	printf("Input Data: ");
	status = isAmountAvailable(&termtemp, accountsDB);
	printf("Expected Result: Amount is not available\n");
	if (status == SERVER_OK)
	{
		printf("Actual Result: Amount is available\n");
	}
	else if (status == LOW_BALANCE)
	{
		printf("Actual Result: Amount is not available\n");
	}
	printf("--------------------------------------\n");
}
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	uint32_t i = sequencenumber - 1000;

	if ( (i<0) || (i >= MAX_ACCOUNT_DB)) {

		return SAVING_FAILED;
	}

	strcpy(transactionsDB[i].cardHolderData.cardHolderName, transData->cardHolderData.cardHolderName);
	strcpy(transactionsDB[i].cardHolderData.cardExpirationDate, transData->cardHolderData.cardExpirationDate);
	strcpy(transactionsDB[i].cardHolderData.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber);

	strcpy(transactionsDB[i].terminalData.transactionDate, transData->terminalData.transactionDate);

	transactionsDB[i].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;
	transactionsDB[i].terminalData.transAmount = transData->terminalData.transAmount;

	transactionsDB[i].transactionSequenceNumber = transData->transactionSequenceNumber;
	transactionsDB[i].transState = transData->transState;

	sequencenumber++;

	listSavedTransactions();

	return SERVER_OK;
}

void saveTransactionTest(void)
{
	ST_transaction_t temptransaction;
	EN_transState_t status;

	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: saveTransaction\n");

	uint8_t test1_n[] = "Osama Mahmoud Hanafy", test1_d[] = "05/25", test1_p[] = "5594454329578106";
	printf("Test Case 1: Name:%s Date:%s PAN:%s\n", test1_n, test1_d, test1_p);
	printf("Test Case 1: 5000 from 12000\n");
	printf("Test Case 1: RUNNING\n");
	printf("Input Data: \n");

	account_ref = 0;
	sequencenumber = 1000;
	temptransaction.transactionSequenceNumber = sequencenumber;
	temptransaction.terminalData.transAmount = 5000;
	temptransaction.terminalData.maxTransAmount = 10000;
	strcpy(temptransaction.cardHolderData.cardHolderName, "Osama Mahmoud Hanafy");
	strcpy(temptransaction.cardHolderData.cardExpirationDate, "05/25");
	strcpy(temptransaction.cardHolderData.primaryAccountNumber, "5594454329578106");
	strcpy(temptransaction.terminalData.transactionDate, "4/11/2022");
	temptransaction.transState = RUNNING;

	status = saveTransaction(&temptransaction);
	printf("\nExpected Result: Saving is ok\n");
	if (status == SERVER_OK)
	{
		printf("Actual Result: Saving is ok\n");
	}
	else if (status == SAVING_FAILED)
	{
		printf("Actual Result: Saving Failed\n");
	}
	printf("--------------------------------------\n");

	uint8_t test2_n[] = "Osama Mahmoud Hanafy", test2_d[] = "05/25", test2_p[] = "5594454311578106";
	printf("Test Case 2: Name:%s Date:%s PAN:%s\n", test2_n, test2_d, test2_p);
	printf("Test Case 2: 5000 from 12000\n");
	printf("Test Case 2: BLOCKED\n");
	printf("Input Data: \n");

	account_ref = 4;
	sequencenumber = 1500;
	temptransaction.transactionSequenceNumber = sequencenumber;
	temptransaction.terminalData.transAmount = 5000;
	temptransaction.terminalData.maxTransAmount = 10000;
	strcpy(temptransaction.cardHolderData.cardHolderName, "Osama Mahmoud Hanafy");
	strcpy(temptransaction.cardHolderData.cardExpirationDate, "05/25");
	strcpy(temptransaction.cardHolderData.primaryAccountNumber, "5594454311578106");
	strcpy(temptransaction.terminalData.transactionDate, "4/11/2022");
	temptransaction.transState = BLOCKED;

	status = saveTransaction(&temptransaction);
	printf("\nExpected Result: Saving Failed\n");
	if (status == SERVER_OK)
	{
		printf("Actual Result: Saving is ok\n");
	}
	else if (status == SAVING_FAILED)
	{
		printf("Actual Result: Saving Failed\n");
	}
	printf("--------------------------------------\n");
}
void listSavedTransactions(void)
{
	uint32_t i = 0;

	while (transactionsDB[i].transactionSequenceNumber != 0)
	{
		printf("\n###############################\n");
		printf("Transaction Sequence Number: %d\n", transactionsDB[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n", transactionsDB[i].terminalData.transactionDate);
		printf("Transaction Amount: %f\n", transactionsDB[i].terminalData.transAmount);
		if (transactionsDB[i].transState == RUNNING) {
			printf("Transaction State: %s\n", "RUNNING");
		}
		else {
			printf("Transaction State: %s\n", "BLOCKED");
		}
		printf("Terminal Max Amount: %f\n", transactionsDB[i].terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transactionsDB[i].cardHolderData.cardHolderName);
		printf("PAN: %s\n", transactionsDB[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n", transactionsDB[i].cardHolderData.cardExpirationDate);
		printf("###############################\n");

		i++;
	}
}

void listSavedTransactionsTest(void)
{
	printf("Tester Name: Osama Mahmoud\n");
	printf("Function Name: listSavedTransactions\n");

	uint8_t test1_n[] = "Osama Mahmoud Hanafy", test1_d[] = "05/25", test1_p[] = "5594454329578106";
	printf("Test Case 1: Name:%s Date:%s PAN:%s\n", test1_n, test1_d, test1_p);
	printf("Test Case 1: 5000 from 12000\n");
	printf("Test Case 1: RUNNING\n");
	printf("Input Data: \n");

	account_ref = 0;
	sequencenumber = 1000;
	transactionsDB[0].transactionSequenceNumber = sequencenumber;
	transactionsDB[0].terminalData.transAmount = 5000;
	transactionsDB[0].terminalData.maxTransAmount = 10000;
	
	strcpy(transactionsDB[0].cardHolderData.cardHolderName, "Osama Mahmoud Hanafy");
	strcpy(transactionsDB[0].cardHolderData.cardExpirationDate, "05/25");
	strcpy(transactionsDB[0].cardHolderData.primaryAccountNumber, "5594454329578106");
	strcpy(transactionsDB[0].terminalData.transactionDate, "4/11/2022");
	transactionsDB[0].transState = RUNNING;

	listSavedTransactions();
	printf("\nExpected Result: List is ok\n");
	if (sequencenumber != 0)
	{
		printf("Actual Result: List is ok\n");
	}
	else
	{
		printf("Actual Result: List Failed\n");
	}
	printf("--------------------------------------\n");

	uint8_t test2_n[] = "Ahmed Mohamed Mahmoud", test2_d[] = "04/23", test2_p[] = "5594454311578106";
	printf("Test Case 2: Name:%s Date:%s PAN:%s\n", test2_n, test2_d, test2_p);
	printf("Test Case 1: 5000 from 12000\n");
	printf("Test Case 2: BLOCKED\n");
	printf("Input Data: \n");

	account_ref = 4;
	sequencenumber = 1001;
	transactionsDB[1].transactionSequenceNumber = sequencenumber;
	transactionsDB[1].terminalData.transAmount = 5000;
	transactionsDB[1].terminalData.maxTransAmount = 10000;

	strcpy(transactionsDB[1].cardHolderData.cardHolderName, "Ahmed Mohamed Mahmoud");
	strcpy(transactionsDB[1].cardHolderData.cardExpirationDate, "04/23");
	strcpy(transactionsDB[1].cardHolderData.primaryAccountNumber, "5594454311578106");
	strcpy(transactionsDB[1].terminalData.transactionDate, "4/11/2022");
	transactionsDB[1].transState = BLOCKED;

	listSavedTransactions();
	printf("\nExpected Result: List failed\n");
	if (sequencenumber != 0)
	{
		printf("Actual Result: List is ok\n");
	}
	else
	{
		printf("Actual Result: List Failed\n");
	}
	printf("--------------------------------------\n");
}
