#include <stdio.h>

double get_balance(){ //Reads balance from file
    
    FILE *dosya;
    dosya = fopen("account.txt", "r");
    
    if(dosya == NULL){
        
        dosya = fopen("account.txt", "w");
        fprintf(dosya, "100.00");
        fclose(dosya);
        return 100.00;
    }
    
    
    double balance;
    
    fscanf(dosya, "%lf", &balance);
    fclose(dosya);

    return balance;
    }

void update_balance(double new_balance){ //Updates balance in the file

    FILE *dosya;
    dosya = fopen("account.txt", "w");
    
    fprintf(dosya, "%2.f", new_balance);
    fclose(dosya);
    }
   
void check_balance(){ //Displays current balance
 
   double balance = get_balance();
   printf("Current balance: %2.f\n", balance);
    
}

void deposit_money() { //Allows user to deposit money

    double amount;
    
    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);
    
       if (amount <= 0) {
        printf("Invalid amount. Amount must be positive.\n");
        return;
    }
    
    double balance = get_balance();
    balance += amount;
    update_balance(balance);
    printf("Deposit successful! New balance: %.2f TRY\n", balance);
}
void withdraw_money(){ //Allows user to withdraw money 

    double amount;
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Invalid amount. Amount must be positive.\n");
        return;
    }

    double balance = get_balance();
    
    if (amount > balance) {
        printf("Insufficient funds! Your balance is only %.2f TRY.\n", balance);
        return;
    }

    balance -= amount;
    update_balance(balance);
    printf("Withdraw successful! New balance: %.2f TRY\n", balance);
}
void menu(){ //Displays the ATM Menu

    int option;

    printf("----- Virtual ATM -----\n");
    printf("1. Check Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Exit\n");
    printf("Choose an option: \n");
    scanf("%d", &option);

switch(option) {
   
    case 1:
    check_balance();
    break;
    
    case 2:
    deposit_money();
    break;
    
    case 3:
    withdraw_money();
    break;
    
    case 4:
    break;
    
    default:
    printf("Invalid option.\n");
}
}

int main() {
    menu();
    return 0;
}