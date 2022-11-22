#include "app.h"


void appStart(void)
{
	ST_transaction_t transaction;
	EN_cardError_t card_status = OK;
	EN_terminalError_t term_status = TERMINAL_OK;
	EN_transState_t trans_status = APPROVED;

	float Max_amount = 10000;

	printf("--------- Payment Application -----------\n");

	while (1)
	{
		printf("************ Terminal Side ****************\n");

		printf("Enter the card Holder Name: ");
		card_status = getCardHolderName(&(transaction.cardHolderData));
		if (card_status == WRONG_NAME) {
			printf("Wrong Name.\n");
			return 0;
		}

		printf("Enter the card EXP Date: ");
		card_status = getCardExpiryDate(&(transaction.cardHolderData));
		if (card_status == WRONG_EXP_DATE) {
			printf("wrong EXP Date.\n");
			return 0;
		}

		printf("ENter the card PAN: ");
		card_status = getCardPAN(&(transaction.cardHolderData));
		if (card_status == WRONG_PAN) {
			printf("wrong PAN.\n");
			return 0;
		}

	

		printf("Enter the transaction Date: ");
		term_status = getTransactionDate(&(transaction.terminalData));
		if (term_status == WRONG_DATE) {
			printf("Wrong Date.\n");
			return 0;
		}

		term_status = isCardExpired(&(transaction.cardHolderData), &(transaction.terminalData));
		if (term_status == EXPIRED_CARD) {
			printf("\nDeclined Expired Card.\n");
			return 0;
		}

		printf("Enter the transaction Amount: ");
		term_status = getTransactionAmount(&(transaction.terminalData));
		if (term_status == INVALID_AMOUNT) {
			printf("\nInvalid Amount.\n");
			return 0;
		}
	
		term_status = setMaxAmount(&(transaction.terminalData), Max_amount);
		if (term_status == INVALID_MAX_AMOUNT) {
			printf("\nInvalid Max Amount.\n");
			return 0;
		}
		term_status = isBelowMaxAmount(&(transaction.terminalData));
		if (term_status == EXCEED_MAX_AMOUNT) {
			printf("\nDeclined Amount Exceeding Limit.\n");
			return 0;
		}

		printf("************ Server Side ****************\n");

		trans_status = recieveTransactionData(&transaction);
		if (trans_status == FRAUD_CARD) {
			printf("Declined Invalid Account.\n");
			return 0;
		}
		else if (trans_status == DECLINED_INSUFFECIENT_FUND) {
			printf("Declined Insuffecient funds.\n");
			return 0;
		}
		else if (trans_status == DECLINED_STOLEN_CARD) {
			printf("Declined Stolen Card.\n");
			return 0;
		}
		else if (trans_status == INTERNAL_SERVER_ERROR) {
			printf("Internal Server error.\n");
			return 0;
		}
		else
		{
			printf("Transaction is Approved.\n");
			return 0;
		}

	}
}