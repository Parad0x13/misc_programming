#include <stdio.h>

float priceToTotalGoal(float totalGoal);

float purchasePrice, salePrice, commision;
float totalPrice, finalGain;
int stockCount;
float increment = 10.00;

int main() {
	commision = 10.00;
	printf("\n\nCalculating with a trade commision of %.2f, when you buy and sell\n", commision);
	printf("Price of stock when purchased: ");
	scanf("%f", &purchasePrice);
	printf("How many stock purchased: ");
	scanf("%i", &stockCount);
	totalPrice = purchasePrice*stockCount+commision;
	printf("Spent a total of: $%.2f(Stock)+$%.2f(Commision) = $%.2f\n", totalPrice-commision, commision, totalPrice);
	printf("\n");

	for(finalGain = 0;finalGain<100+increment;finalGain+=increment) {
		printf("To make $%.2f the price must be %f\n", finalGain, priceToTotalGoal(totalPrice+finalGain));
	}
}

float priceToTotalGoal(float totalGoal) {
	return (totalGoal+commision)/stockCount;
}
