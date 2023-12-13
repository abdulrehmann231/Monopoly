#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define MAX_PLAYERS 8

int RollDIce(int);  // action for dice roll and returns its value
void spaceeffect(int); //to perform functions when you land on space
void propertyFunctions(int,int); //what happens when you land on a property
void monopolyCheck(int); //checks if monopoly has been made on a group 
void printGameStats(int); //printing game stats like money and position of players
void processPayments(int, int, int);//process payments transferred in game three inputs first player who gets money second who loses money and third is amount -1 means bank
void sell_property(int);//to sell property to bank or a player
void diceeffect(int);

void ToJail(int); //for sending player to jail and ending players 
void build_house(int); //builds house on  a property
void trade(int); //to trade properties between players
int utilityRent(int, int); //calculates rent on companies using dice roll value
void chance(int); //what hapens when you land on CHANCE
void community_chest(int); //what happens when you land on COMMUNITY CHEST
void shuffleChance(void); //shuffles chance cards
void shuffleCommunityCards(void); //shuffles community chest cards
int determine_blanks(void); //determines what goes in the blank_count array, returns the size of the biggest name
int loan(int);//when a player wants to take loan;
void payloan(int);//when a player wants to pay loan
void printMonopolyBoard(void);


int RollDIce_value; //used to store output of RollDIce for the case of the player in jail
char PlayerNames[MAX_PLAYERS][100]; 
int blank_count[MAX_PLAYERS]; //how many spaces for proper even spacing for each name
int player_cash[MAX_PLAYERS] = {-1, -1, -1, -1, -1, -1, -1, -1}; //set to -1 if the player is eliminated from the game
int player_space[MAX_PLAYERS];//current space of each player
int property_ownership[28]; //-1 means bank
int monopoly_status[MAX_PLAYERS] = {0, 0, 0, 0, 0, 0, 0, 0};
int price[] = {60, 60, 100, 100, 120, 140, 140, 160, 180, 180, 200, 220, 220, 240, 260, 260, 280, 300, 300, 320, 350, 400, 200, 200, 200, 200, 150, 150}; //price of each  property
int GroupStaringProp[] = {0, 2, 5, 8, 11, 14, 17, 20}; //starting properties of each group
int house_costs[8] = {50, 50, 100, 100, 150, 150, 200, 200};//each index represents cost of house in the respective group
int house_count[28]; //number of houses on each property
int goojf_ownership[] = {-1, -1};
//get out of jail free cards: 0th is chance 1th is cc

int jail_status[MAX_PLAYERS]; //stores if player is in jail or not
char space_names[][22] = {"GO", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance", "Vermont Avenue", "Connecticut Avenue", "Just Visiting!", "St. Charles Place", "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad", "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue", "FREE PARKING", "Kentucky Avenue", "Chance", "Indiana Avenue", "Illinois Avenue", "B & O Railroad", "Atlantic Avenue", "Ventnor Avenue", "Water Works", "Marvin Gardens", "GO TO JAIL", "Pacific Avenue", "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line", "Chance", "Park Place", "Luxury Tax", "Boardwalk"};//names of spaces or boxes on board
char prop_names[][22] = {"Mediterranean Avenue", "Baltic Avenue", "Oriental Avenue", "Vermont Avenue", "Connecticut Avenue", "St. Charles Place", "States Avenue", "Virginia Avenue", "St. James Place", "Tennessee Avenue", "New York Avenue", "Kentucky Avenue", "Indiana Avenue", "Illinois Avenue", "Atlantic Avenue", "Ventnor Avenue", "Marvin Gardens", "Pacific Avenue", "North Carolina Avenue", "Pennsylvania Avenue", "Park Place", "Boardwalk", "Reading Railroad", "Pennsylvania Railroad", "B & O Railroad", "Short Line", "Electric Company", "Water Works"}; //array for properties 
int chance_deck[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};//15 cards in chance
int cc_deck[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};//15 cards in cc
int loan_check[MAX_PLAYERS]={0,0,0,0,0,0,0,0};//to check if a player has taken loan;
int player_count = 0;
int currentplayers=0; //how many players
int longest_name; //length in chars of the longest entered name
int RollDIce_over;
int checkloan;//check the return value of loan function
int doubles_count; //counts number of doubles rolled (if this number exceeds 2, go directly to jail)
int jail_roll_count[MAX_PLAYERS]; //counts number of attempts to roll out of jail for each player
int chance_remaining_cards = 15;
int cc_remaining_cards = 15;
int num;//to display options

int main(){
	int player_turn = 0; 
	int int_input,check1,check2,check3; 
	int TurnEnd = 0; //turn ended??
	int case_flag = 1; //used to consider options only available from jail
	int *p = &player_cash[0]; //pointer for checking player cash or to check if player is eliminated
	int i;
	
	
	for(i = 0; i < 28; i++){ 
		property_ownership[i] = -1;
		house_count[i] = 0;
	}

	srand((int) time(NULL));

	shuffleChance();
	shuffleCommunityCards();

	while(1){
		printf("How many players? ");
		fflush(stdout);
		scanf("%d", &player_count);
		if(player_count > 1 && player_count <= MAX_PLAYERS) break;
		else printf("Please enter a player count between 2 and MAX_PLAYERS\n");
	}

     currentplayers = player_count;
	for(i = 0; i < player_count; i++){ //initialize each player
		printf("Enter a name for Player %d: ", i+1);
		fflush(stdout);
		scanf("%s", PlayerNames[i]);
		player_cash[i] = 1500; //player is given $1500 starting cash
		player_space[i] = 0; //player is initialized on the first space (GO)
		jail_status[i] = 0; //player begins outside of jail
	}
     
     
	longest_name = determine_blanks();

	// Getting into Gameplay //
	// for now, assume players play in order - TODO: add roll to get player order
	while(currentplayers > 1){ //when current players  == 1, the game is over (because there is only one person left alive)
	
	       
		system("cls");
		printf("\n\n\n\n");
		TurnEnd = 0;
		RollDIce_over = 0;
		doubles_count = 0; //reset doubles counter each turn
          
		while(!TurnEnd && player_cash[player_turn] >= 0){ //ensure the turn is not over AND that the player has not gone bankrupt
			
			printf("  ");
			
			if(doubles_count && !RollDIce_over) printf("You rolled doubles, %s. Go Again!", PlayerNames[player_turn]); //subsequent roll after getting doubles
			else printf("It is %s's turn!", PlayerNames[player_turn]); //first roll
			
			printf("  ");
			
			printf("\n");

			printf("\n%s is ", PlayerNames[player_turn]); //let the player know what space they are on at the beginning of each turn
			if(jail_status[player_turn]) printf("in JAIL\n", PlayerNames[player_turn]);
			else printf("on %s\n", space_names[player_space[player_turn]]);

			printf("\n");
			while(1){
				check1=0;
				check2=0;
				check3=0;
				num=1;
				if(!RollDIce_over){
				printf("%d) Roll the Dice",num++);
			        if(jail_status[player_turn]){ //if player is in jail
				    	printf(" to get out of JAIL\n"); 
				    	check1=1;
			    		printf("%d) Pay $50 Fine to get out of JAIL",num++);
			    		if(goojf_ownership[0] == player_turn ||  goojf_ownership[1] == player_turn){
			    			check2=1;
			    			printf("\n%d) Use Get Out of Jail Free Card to get out of JAIL",num++);
						}
			    	}
				printf("\n");
		    	}
		    	printf("%d) View Game Stats: Money\n",num++);
		    	printf("%d) View Game Stats: Property Ownership and Information\n",num++);
		    	printf("%d) View Game Stats: Board Info\n",num++);
		    	printf("%d) Build/Sell Houses/Hotels\n",num++); 
		    	printf("%d) Initiate a Trade Deal\n",num++);
		    	printf("%d) Sell a property\n",num++);
		    	if (loan_check[player_turn]==1){
		    		printf("%d) Pay Loan\n",num++);
		    		check3=1;
		    	}
		    	if(RollDIce_over) printf("%d) End Turn\n",num++);
		    	fflush(stdin);
		    	scanf("%d", &int_input);
		        printf("\n");
		    	if (!RollDIce_over){
		    		if (check1==1){
		         		if (check2==1 && check3==1){
		    	    	   	if (int_input>0 && int_input<=10) break;
				    	}
		    	     	else if (check2==1 || check3==1){
		    	    		if (int_input>0 && int_input<=9) break;
			    		}
				    	else {
				    		if (int_input>0 && int_input<=8) break;
				    	}
			    	}
		    		else {
			    		if (check3==1){
				    		if (int_input>0 && int_input<=8) break;
				    	}
			     		else {
				    		if (int_input>0 && int_input<=7) break;
			 	    	} 
			    	}
				}
				else {
					if (check3==1){
						if (int_input>0 && int_input<=8) break;
					}
					else {
						if (int_input>0 && int_input<=7) break;
					}
				}
				printf("Wrong input! Please enter a number between 1 and %d\n\n",--num);
			}
            printf("\n");
			switch(int_input){
                case 1:
                	if (!RollDIce_over){
                		diceeffect(player_turn);
					}
					else {
						printGameStats(int_input);
					}
					break;
				case 2:
				    if (!RollDIce_over){
				    	if (check1==1){
				        	if(jail_status[player_turn]){ //player has payed to get out of jail
					     		processPayments(-1, player_turn, 50);
					     		jail_status[player_turn] = 0;
					    		printf("%s is no longer in JAIL\n\n", PlayerNames[player_turn]);
					    	}
				    	}
			    		else {
				    		printGameStats(int_input-1);
				    	}	
					}
					else {
						printGameStats(int_input);
					}
					break;
				case 3:
				    if (!RollDIce_over){
				    	if (check2==1){
				    		if(jail_status[player_turn] && (goojf_ownership[0] == player_turn || goojf_ownership[1] == player_turn)){ 
					
					    		if(goojf_ownership[0] == player_turn){
						     	    goojf_ownership[0] = -1;	
						    	}
						    	else if(goojf_ownership[1] == player_turn){
						    	    goojf_ownership[1] = -1;	
					    		}
						    	jail_status[player_turn] = 0;
							printf("%s is no longer in JAIL\n\n", PlayerNames[player_turn]);
					    	}
						}
						else {
			                if (check1==1){
			                	printGameStats(int_input-2);
							}
							else {
								printGameStats(int_input-1);
							}
						}
					}
					else {
						printGameStats(int_input);
					}
					break;		
                case 4:
                	if (!RollDIce_over){
                		if (check1==1){
                			if (check2==1){
                				printGameStats(int_input-3);
							}
							else {
								printGameStats(int_input-2);
							}
						}
						else{
							printGameStats(int_input-1);
						}
					}
					else{
						build_house(player_turn);
					}
					break;
				case 5:
					if (!RollDIce_over){
						if (check1==1){
							if (check2==1){
								printGameStats(int_input-3);
							}
							else {
								printGameStats(int_input-2);
							}
						}
						else {
							build_house(player_turn);
						}
					}
					else {
						trade(player_turn);
					}
					break;
				case 6:
					if (!RollDIce_over){
						if (check1==1){
							if (check2==1){
								printGameStats(int_input-3);
							}
							else {
								build_house(player_turn);
							}
						}
						else {
							trade(player_turn);
						}
					}
					else {
						sell_property(player_turn);
					}
					break;
				case 7:
					if (!RollDIce_over){
						if (check1==1){
							if (check2==1){
								build_house(player_turn);
							}
							else {
								trade(player_turn);
							}
						}
						else {
							sell_property(player_turn);
						}
					}
					else {
						if (check3==1){
							if(loan_check[player_turn]==1){
				        	    payloan(player_turn);
				        	    break;
                    	  	}
				        	printf("Invalid input \n");
                        	break;
						}
						else {
							TurnEnd = 1;
				    		while(1){
					    		player_turn++;
					    		p++;
					    		if(player_turn >= MAX_PLAYERS){
					    			player_turn = 0;
					    			p = &player_cash[0];
					    		}
					    		if(*p >= 0) break;
					    	}
						    printf("\n");
						}
					}
					break;
				case 8:
					if (!RollDIce_over){
						if (check1==1){
							if (check2==1){
								trade(player_turn);
							}
							else {
								sell_property(player_turn);
							}
						}
						else {
							if (check3==1){
								if(loan_check[player_turn]==1){
				        	    payloan(player_turn);
				        	    break;
                    	     	}
				            	printf("Invalid input \n");
                            	break;
							}
						}
					}
					else {
						if (check3==1){
							TurnEnd = 1;
				    		while(1){
					    		player_turn++;
					    		p++;
					    		if(player_turn >= MAX_PLAYERS){
					    			player_turn = 0;
					    			p = &player_cash[0];
					    		}
					    		if(*p >= 0) break;
					    	}
						    printf("\n");
						}
					}
					break;
				case 9:
					if (!RollDIce_over){
						if (check1==1){
							if (check2==1){
								sell_property(player_turn);
							}
							else {
								if (check3==1){
							    	if(loan_check[player_turn]==1){
				        	           payloan(player_turn);
				                	    break;
                    	        	}
				                	printf("Invalid input \n");
                                	break;
						    	}
							}
						}
					}
					break;
				case 10:
					if (!RollDIce_over){
						if (check1==1&&check2==1&&check3==1){
						 	if(loan_check[player_turn]==1){
				                payloan(player_turn);
				          	    break;
                    	   	}
				          	printf("Invalid input \n");
                          	break;	
						}
					}
					break;
//				case 5:
//				case 6:
//					printGameStats(int_input-3);
//					break;
//
//				case 7:
//					build_house(player_turn);
//					
//					break;
//
//				case 8:
//					trade(player_turn);
//					break;
//                
//                case 9:
//                	sell_property(player_turn);
//                	break;
                
//                case 10:
//                	if(loan_check[player_turn]==1)
//					{
//					  payloan(player_turn);
//                	  break;	
//					}
//					printf("Invalid input \n");
//                	break;
//				default:
//					if(!RollDIce_over){
//
//						if(int_input == 2 && jail_status[player_turn]){ //player has payed to get out of jail
//							processPayments(-1, player_turn, 50);
//							jail_status[player_turn] = 0;
//							printf("%s is no longer in JAIL\n\n", PlayerNames[player_turn]);
//						}
//
//						else if(int_input == 3 && jail_status[player_turn] && (goojf_ownership[0] == player_turn || goojf_ownership[1] == player_turn)){ 
//					
//							if(goojf_ownership[0] == player_turn) 
//							{
//							  goojf_ownership[0] = -1;	
//							}
//							else if(goojf_ownership[1] == player_turn)
//							{
//							  goojf_ownership[1] = -1;	
//							}
//							jail_status[player_turn] = 0;
//							printf("%s is no longer in JAIL\n\n", PlayerNames[player_turn]);
//						}
//
//					}
//
//					else{
//						TurnEnd = 1;
//						while(1){
//							player_turn++;
//							p++;
//							if(player_turn >= MAX_PLAYERS){
//								player_turn = 0;
//								p = &player_cash[0];
//							}
//							if(*p >= 0) break;
//						}
//						printf("\n");
//					}

			}
			if(player_cash[player_turn] <0 )
			{
				break;
			}
		}
		if(player_cash[player_turn] < 0)
		{
			player_turn++;
			p++;
			if(player_turn >= MAX_PLAYERS){
			player_turn = 0;
			p = &player_cash[0];
		 }
							
		}
	}

	for(i = 0; i < MAX_PLAYERS; i++){
		if(player_cash[i] >= 0) break;
	}
	printf("Congratulations, %s! You are the winner of Monopoly!\n", PlayerNames[i]); 
	

	return 0;
}

int RollDIce(int player){
	int dice1, dice2;
	
	if(doubles_count == 2) printf("Be Careful! One more doubles and you go to JAIL...\n"); 
	printf("%s Roll!", PlayerNames[player]);
	getchar();
	printf("\n");

	dice1 = (rand() % 6)+1; //determine dice 1
	dice2 = (rand() % 6)+1; //determine dice 2

	//figure out dice_dots
	printf("DICE 1 : %d\n",dice1);
    printf("DICE 2 : %d\n",dice2);
	
	if(dice1 == dice2){
		doubles_count++;
		printf("Doubles! ");
	}
	else RollDIce_over = 1;
	printf("%s rolled %d", PlayerNames[player], dice1+dice2);
	if(doubles_count == 3){
		printf("\nYou rolled 3 doubles. %s, GO TO JAIL\n", PlayerNames[player]);
		ToJail(player);
	}
	
	getchar();
	

	return (dice1+dice2);
}

void diceeffect(int player_turn){
	if (!RollDIce_over){
		RollDIce_value = RollDIce(player_turn); //player has rolled the dice (either to attempt to escape jail or to move)

		if(doubles_count < 3){ //ensure that the player did not just roll doubles a third time and went to jail
			if(jail_status[player_turn]){ //is the player in jail?

				if(RollDIce_over){
					jail_roll_count[player_turn]++;
					if(jail_roll_count[player_turn] < 3) printf("%s shall remain in JAIL\n\n", PlayerNames[player_turn]); //did not roll doubles
					if(jail_roll_count[player_turn] == 3){ //player has attempted to roll doubles unsuccessfully 3 times and is forced to pay the fine
						printf("%s, you have attempted to roll doubles unsuccessfully 3 times and must now pay the $50 fine\n", PlayerNames[player_turn]);
						processPayments(-1, player_turn, 50);
		    			printf("%s is no longer in JAIL\n\n", PlayerNames[player_turn]);
						jail_status[player_turn] = 0;
						RollDIce_value = RollDIce(player_turn); //reroll now that out of jail
					}
				}
				else{
					printf("%s is no longer in JAIL\n", PlayerNames[player_turn]);
					jail_status[player_turn] = 0;
					RollDIce_over = 1; //to ensure rolling a doubles only gets you out of jail and nothing else
				}
			}

			if(!jail_status[player_turn]){
				player_space[player_turn] += RollDIce_value;
				if(player_space[player_turn] > 39){
		    		printf("%s passed GO! Collect $200\n", PlayerNames[player_turn]);
					processPayments(player_turn, -1, 200); //player passed go (collect $200)
					player_space[player_turn] %= 40;
				}
				printf("\n%s landed on %s\n", PlayerNames[player_turn], space_names[player_space[player_turn]]);
				spaceeffect(player_turn);
			}
		}

		else printf("\n");
	}
}

void spaceeffect(int player){
	int space = player_space[player];

	switch(space){
		case 0:
			//if land on go = $400 mode is on, set accordinly
			break;

		case 10:
			printf("You are just visiting :)");
			break;

		case 20:
			printf("Relax, its Free Parking!");
			
			break;

		case 30:
			ToJail(player);
			break;

		case 7:
		case 22:
		case 36:
			chance(player);
			break;

		case 2:
		case 17:
		case 33:
			community_chest(player);
			break;

		case 4:
			printf("You must pay $200 income tax\n");
			processPayments(-1, player, 200);
			break;

		case 38:
			printf("You must pay $75 luxury tax\n");
			processPayments(-1, player, 75);
			break;

		default:
			propertyFunctions(player,0);

	}

	printf("\n\n");
	
	return;
}

int loan(int player){

	int takeloan;//to take input of user whether he wants to take loan or not
    while (takeloan!=1 && takeloan!=2){
    	printf("Do you want to take loan of $300 from bank ?\n1)Yes 2)No : ]");
        fflush(stdin);
        scanf("%d",&takeloan);
	}
    if (takeloan==1){
    	if (loan_check[player]==0){
    	loan_check[player]++;
    	player_cash[player]+=300;
    	return 1;
    	}
    	else {
    		printf("You have already taken a loan . Pay it back first!\n");
    		return 0;
		}
    }
    else{
    	return 0;
	}
}

void propertyFunctions(int player, int fromchance){
	//printf("WIP");

	// Property Array Guide //
	// 00 = Mediterranean Avenue                                    --> 0 property/monopoly group
	// 01 = Baltic Avenue                                           --> 0 property/monopoly group
	// 02 = Oriental Avenue                                         --> 1 property/monopoly group
	// 03 = Vermont Avenue                                          --> 1 property/monopoly group
	// 04 = Connecticut Avenue                                      --> 1 property/monopoly group
	// 05 = St. Charles Place                                       --> 2 property/monopoly group
	// 06 = States Avenue                                           --> 2 property/monopoly group
	// 07 = Virginia Avenue                                         --> 2 property/monopoly group
	// 08 = St. James Place                                         --> 3 property/monopoly group
	// 09 = Tennessee Avenue                                        --> 3 property/monopoly group
	// 10 = New York Avenue                                         --> 3 property/monopoly group
	// 11 = Kentucky Avenue                                         --> 4 property/monopoly group
	// 12 = Indiana Avenue                                          --> 4 property/monopoly group
	// 13 = Illinois Avenue                                         --> 4 property/monopoly group
	// 14 = Atlantic Avenue                                         --> 5 property/monopoly group
	// 15 = Ventnor Avenue                                          --> 5 property/monopoly group
	// 16 = Marvin Gardens                                          --> 5 property/monopoly group
	// 17 = Pacific Avenue                                          --> 6 property/monopoly group
	// 18 = North Carolina Avenue                                   --> 6 property/monopoly group
	// 19 = Pennsylvania Avenue                                     --> 6 property/monopoly group
	// 20 = Park Place                                              --> 7 property/monopoly group
	// 21 = Boardwalk                                               --> 7 property/monopoly group
	// 22 = Reading Railroad (here begins non-buildable properties) 
	// 23 = Pennsylvania Railroad                                   
	// 24 = B & O Railroad                                          
	// 25 = Short Line                                              
	// 26 = Electric Company                                        
	// 27 = Water Works                                             

	
	int rent[][6] = {{2, 10, 30, 90, 160, 250}, {4, 20, 60, 180, 320, 450}, {6, 30, 90, 270, 400, 550}, {6, 30, 90, 270, 400, 550}, {8, 40, 100, 300, 450, 600}, {10, 50, 150, 450, 625, 750}, {10, 50, 150, 450, 625, 750}, {12, 60, 180, 500, 700, 900}, {14, 70, 200, 550, 750, 950}, {14, 70, 200, 550, 750, 950}, {16, 80, 220, 600, 800, 1000}, {18, 90, 250, 700, 875, 1050}, {18, 90, 250, 700, 875, 1050}, {20, 100, 300, 750, 925, 1100}, {22, 110, 330, 800, 975, 1150}, {22, 110, 330, 800, 975, 1150}, {24, 120, 360, 850, 1025, 1200}, {26, 130, 390, 900, 1100, 1275}, {26, 130, 390, 900, 1100, 1275}, {28, 150, 450, 1000, 1200, 1400}, {35, 175, 500, 1100, 1300, 1500}, {50, 200, 600, 1400, 1700, 2000}}; 
	//rent index guide:
	//0 = rent with 0 houses/1 railroad
	//1 = rent with 1 house/2 railroads
	//2 = '    '    2 '     3 '
	//3 = '    '    3 '     4 '
	//4 = '    '    4 '
	//5 = rent with hotel
	
	
	
	//to add: mortgage prices
	
	int space_conversion[][3] = {{1, 0, 0}, {3, 1, 0}, {5, 22, 8}, {6, 2, 1}, {8, 3, 1}, {9, 4, 1}, {11, 5, 2}, {12, 26, 9}, {13, 6, 2}, {14, 7, 2}, {15, 23, 8}, {16, 8, 3}, {18, 9, 3}, {19, 10, 3}, {21, 11, 4}, {23, 12, 4}, {24, 13, 4}, {25, 24, 8}, {26, 14, 5}, {27, 15, 5}, {28, 27, 9}, {29, 16, 5}, {31, 17, 6}, {32, 18, 6}, {34, 19, 6}, {35, 25, 8}, {37, 20, 9}, {39, 21, 9}};
	//space_conversion index guide:
	//0 = literal space
	//1 = space according to property index
	//2 = group the property belongs to (8 is railroads, 9 is utilities)
	
	int space;
	int group;
	int num_railroads; //number of railroads rr_owner owns
	int final_cost; //final cost assuming player owns the property after determining houses and monopolies
	int done = 0; //used to end this menu if options 1 or 2 are submitted
	int input;
	int i = 0;
    char title[10]; 
 
	while(1){
		if(player_space[player] == space_conversion[i][0]){ //search for space that player landed on
			space = space_conversion[i][1]; //convert it from the array
			group = space_conversion[i][2]; //store the group that the property belongs to
			break;
		}
		i++;
	}

	if(property_ownership[space] == -1){ //property owned by bank, ask player if they would like to buy it
		while(!done){
			printf("%s is unowned! Would you like to buy it, %s?\n\n", space_names[player_space[player]], PlayerNames[player]);
			while(1){
	    	   	printf("1) Pay $%d to buy the property\n", price[space]);
	    		printf("2) Decline\n");
	     		printf("3) View Property Info\n");
	    		printf("4) View Game Stats: Money\n");
	    		printf("5) View Game Stats: Property Ownership and Information\n");
	     		printf("6) View Game Stats: Board Info\n");
	     		num=7;
	   			if (loan_check[player]==1){
	     			printf("7) Pay Loan\n");
	     			num++;
	     		}
		    	fflush(stdout);
		    	scanf(" %d", &input); 
		    	printf("\n");
		    	if (input>0 && input<num){
		    		break;
				}
				else {
					printf("Wrong input! Please enter between 1 and %d\n\n",--num);
				}
			}
			switch(input){
				case 1:
					if(price[space] > player_cash[player]) 
					{
					 printf("%s, you do not have enough money to buy this property!\n\n", PlayerNames[player]);
					 	checkloan=loan(player);
						if (checkloan==1){
							processPayments(-1, player, price[space]);
				    		property_ownership[space] = player;
				    		//change_color(4);
				    		printf("%s is now the owner of %s", PlayerNames[player], space_names[player_space[player]]);
				    		//change_color(0);
				    		if(space <= 21) monopolyCheck(space); //function will check if a monopoly has been made and update the monopoly_status array accordingly
			    			done++;
						}
					}

					else{
						processPayments(-1, player, price[space]);
						property_ownership[space] = player;
	
						printf("%s is now the owner of %s", PlayerNames[player], space_names[player_space[player]]);
	
						if(space <= 21) monopolyCheck(space); //function will check if a monopoly has been made and update the monopoly_status array accordingly
						done++;
					}
					break;

				case 2:
					done++;
					break;

				case 4:
				case 5:
				case 6:
					printGameStats(input-3);
					break;
				case 7:
					if (loan_check[player]==1){
                		payloan(player);
					}
                	break;	
				
				default:
					//todo  add houses cost and remove mortgages
					
					if(space <=21)
					{
						strcpy(title,"PROPERTY");
					}
					else if(space < 26)
					{
						strcpy(title,"RAILROAD");
					}
					else
					{
						strcpy(title,"UTILITY");
					}
					
					printf("%s costs $%d to own\n\n", space_names[player_space[player]], price[space]);
					printf("     \%s\n%s\nRent $%d\nWith 1 House      $ %d.\nWith 2 Houses      %s%d.\nWith 3 Houses      %s%d.\nWith 4 Houses      %d\n     With Hotel $%d.\n     Houses cost $%d. each\nHotels, $%d. plus 4 houses\"\n\n",title, space_names[player_space[player]], rent[space][0], rent[space][1], (rent[space][2] < 100 ? " " : ""), rent[space][2], (rent[space][3] < 100 ? " " : ""), rent[space][3], rent[space][4], rent[space][5], house_costs[group], house_costs[group]);
	

			}
		}
	}

	else{
		if(property_ownership[space] == player) printf("Relax, you own this property\n"); //if player owns the property
		else{ //player does not own the property
			final_cost = 0; //for safety, start final_cost at 0

			if(space <= 21){ //calculate rent on a colored property
				final_cost = rent[space][house_count[space]]; //calculate how much it costs to land on this property
				if(!house_count[space] && monopoly_status[group]) final_cost*=2; //double this if it is a monopoly with 0 houses
			}

			else if(space <= 25){ //calculate rent on a railroad
				num_railroads = 0;
				for(i = 22; i <= 25; i++){ //check ownership on each railroad
					if(property_ownership[i] == property_ownership[space]) num_railroads++;
				}

				
				final_cost=1;
	             while(num_railroads-1 > 0)
				 {
		           final_cost*=2;
		           num_railroads--;
	             }
	             final_cost = final_cost * 25;
				if(fromchance ==1)
				{
					final_cost = final_cost *2;
				}
			}

			else{
				num_railroads = 0; //re-use num_railroads (this time, think of it as num_utilities)
				if(property_ownership[26] == property_ownership[space]) num_railroads++;
				if(property_ownership[27] == property_ownership[space]) num_railroads++;
				final_cost = utilityRent(4+6*(num_railroads-1), RollDIce_value);
				if(fromchance ==1)
				{
					final_cost = utilityRent(final_cost,RollDIce_value);
				}
			}

			printf("%s owns %s. %s owes %s $%d\n", PlayerNames[property_ownership[space]], space_names[player_space[player]], PlayerNames[player], PlayerNames[property_ownership[space]], final_cost); //final_cost finalized, tell user, then make processPayments
			processPayments(property_ownership[space], player, final_cost); 
		}
	}

	return;
}

void monopolyCheck(int space){
	int flag = 0; //set to 1 if a monopoly has been made
	int size; //is it a 2 or 3 property group?
	int group; //what group does this space belong to?
	char group_name[][13] = {"PURPLE/BROWN", "LIGHT BLUE", "PINK", "ORANGE", "RED", "YELLOW", "GREEN", "DARK BLUE"};
	int owner; //owner of the property (by int value)
	int i;

	switch(space){
		case 0:
		case 1:
			size = 2;
			space = 0;
			group = 0;
			break;
		case 2:
		case 3:
		case 4:
			size = 3;
			space = 2;
			group = 1;
			break;
		case 5:
		case 6:
		case 7:
			size = 3;
			space = 5;
			group = 2;
			break;
		case 8:
		case 9:
		case 10:
			size  = 3;
			space = 8;
			group = 3;
			break;
		case 11:
		case 12:
		case 13:
			size = 3;
			space = 11;
			group = 4;
			break;
		case 14:
		case 15:
		case 16:
			size  = 3;
			space = 14;
			group = 5;
			break;
		case 17:
		case 18:
		case 19:
			size  = 3;
			space = 17;
			group = 6;
			break;
		case 20:
		case 21:
			size = 2;
			space = 20;
			group = 7;
			break;
		default:
			printf("ERROR: invalid argument\n");
			return;
	}

	owner = property_ownership[space];

     if(owner == -1)
      {
   	      monopoly_status[group] = 0;	   	
   	      return;
      }

	for(i = size; i >= 2; i--){ //check if each property is owned by the same person
		space++;
		if(owner != property_ownership[space] ) 
		{
	      monopoly_status[group] = 0;	   	
	      return;
	  	}
	}

	//a monopoly has been made:
	monopoly_status[group] = 1;
	printf("\n%s now has a monopoly on the %s property group!\n", PlayerNames[property_ownership[space]], group_name[group]);

	return;
}
void payloan(int player){
	if (player_cash[player]>=300){
		player_cash[player]-=300;
		loan_check[player]=0;
	}
	else{
		printf("You dont have enough money to pay the loan!\n");
	}
}

void printGameStats(int option){
	int group;
	int i;
	int j;

	
	
	int prop_blanks[] = {1, 8, 6, 7, 3, 4, 8, 6, 6, 5, 6, 6, 7, 6, 6, 7, 7, 7, 0, 2, 11, 12, 5, 0, 7, 11, 5, 10};
	int color_blanks[] = {0, 2, 8, 6, 9, 6, 7, 3};

	printf("\n");

	switch(option){
		case 1:
			for(i = 0; i < MAX_PLAYERS; i++){
				if(player_cash[i] >= 0){
					printf("%s ", PlayerNames[i]);
					for(j = 0; j < blank_count[i]; j++){
						printf(" ");
					}
					printf("has $%d", player_cash[i]);
	
					printf("    ");
	
					printf("\n");
				}
			}
			printf("\n");
			break;

		case 2:
			for(i = 0; i < 28; i++){
				group = -1; //reset group each loop
				if(i == 0){
					printf("PROPERTIES\n");
				}
				if(i >= 0 && i < 22 && property_ownership[i] != -1){
					if(i <= 1){
	
						group = 0;
					}
					else if(i <= 4){
	
						group = 1;
					}
					else if(i <= 7){
	
						group = 2;
					}
					else if(i <= 10){
	
						group = 3;
					}
					else if(i <= 13){
	
						group = 4;
					}
					else if(i <= 16){
	
						group = 5;
					}
					else if(i <= 19){
	
						group = 6;
					}
					else if(i <= 21){
	
						group = 7;
					}
				}
				else if(i == 22) printf("RAILROADS\n");
				else if(i == 26) printf("UTILITIES\n");
				printf("%s: ", prop_names[i]);
				for(j = 0; j < prop_blanks[i]; j++){
					printf(" ");
				}
				printf("owned by %s", (property_ownership[i] >= 0 ? PlayerNames[property_ownership[i]] : "the bank")); 
				if(i < 22){
					printf("; ");
					for(j = 0; j < (property_ownership[i] >= 0 ? blank_count[property_ownership[i]] : longest_name-8); j++){
						printf(" ");
					}
					printf("from group ");
				}
				if(i <= 1) printf("PURPLE/BROWN");
				else if(i <= 4) printf("LIGHT BLUE");
				else if(i <= 7) printf("PINK");
				else if(i <= 10) printf("ORANGE");
				else if(i <= 13) printf("RED");
				else if(i <= 16) printf("YELLOW");
				else if(i <= 19) printf("GREEN");
				else if(i <= 21) printf("DARK BLUE");
				if(group >= 0 && monopoly_status[group]){ //the property has a monopoly, so list the number of houses
					printf("; ");
					for(j = 0; j < color_blanks[group]; j++){
						printf(" ");
					}
					printf("%d house%s", house_count[i], (house_count[i] != 1 ? "s" : ""));
				}

				if(property_ownership[i] >= 0){ //color flag to show ownership
					//change_color(player_colors[property_ownership[i]]);
					printf("    ");
				}
				//try to implement showing rent/price later
				//change_color(13); //reset bg color
				//change_color(0); //reset color
				printf("\n");
			}

			printf("GET OUT OF JAIL FREE CARDS\n");
			for(i = 0; i < 2; i++){
				if(!i) printf("Chance Get Out of Jail Free Card:          owned by ");
				else   printf("Community Chest Get Out of Jail Free Card: owned by ");

				if(goojf_ownership[i] >= 0){
					printf("%s", PlayerNames[goojf_ownership[i]]);
					//change_color(player_colors[goojf_ownership[i]]);
					printf("    ");
					//change_color(13);
				}
				else printf("the bank");
				printf("\n");
			}
			printf("\n");
			break;

		case 3:
            
            printMonopolyBoard();
			for(i = 0; i < MAX_PLAYERS; i++){
				if(player_cash[i] >= 0){
					printf("%s ", PlayerNames[i]);
					for(j = 0; j < blank_count[i]; j++){
						printf(" ");
					}
					printf("is ");
					if(jail_status[i]) printf("in JAIL (11th space)"); //in case the player is in jail
					else{
						printf("on the ");
						if(player_space[i]+1 < 10) printf(" ");
						printf("%d", player_space[i]+1);
						switch(player_space[i]){
							case 0:
							case 20:
							case 30:
								printf("st");
								break;
							case 1:
							case 21:
							case 31:
								printf("nd");
								break;
							case 2:
							case 22:
							case 32:
								printf("rd");
								break;
							default:
								printf("th");
						}
						printf(" space on the board: %s", space_names[player_space[i]]);
					}
					//change_color(player_colors[i]);
					printf("    ");
					//change_color(13);
					printf("\n");
				}
			}
			printf("\n");
			break;

		default:
			return;

	}

	return;
}

void processPayments(int gaining, int losing, int amount){ //to do: add intricacies for bankruptcies
	/*
	player_cash[player] += amount;

	printf("%s now has $%d\n", PlayerNames[player], player_cash[player]);

	*/
	int i;
	int flag = 0; //for use on goojf repossession

	if(losing != -1){ //if a player is losing money...
		if(amount <= player_cash[losing]){
			player_cash[losing] -= amount;
			//change_color(2);
			printf("%s now has $%d", PlayerNames[losing], player_cash[losing]);
			//change_color(0);
			printf("\n");
		}
		else{ //if the player cannot pay what he owes
			//to do: write a function to deal with debt (for now, assumes player goes bankrupt)
			 checkloan=loan(losing);
			if (checkloan==1){
				if(amount <= player_cash[losing]){
	        		player_cash[losing] -= amount;
	        		//change_color(2);
	        		printf("%s now has $%d", PlayerNames[losing], player_cash[losing]);
	        		//change_color(0);
	        		printf("\n");
    	    	}//if amount is still greater than players cash,it will not go to else mode..CHECK.
		 	}
		 	else
			 {
			 	
			 
			flag++;
			amount = player_cash[losing];
			player_cash[losing] = -1; //set to -1 to express that this player is no longer in the game (will be changed for cases of debt)
			currentplayers--; //there is 1 less player in the game now
			printf("%s has gone BANKRUPT! %s will repossess all of their belongings:\n", PlayerNames[losing], (gaining >= 0 ? PlayerNames[gaining] : "The bank"));
			if(gaining != -1){ //player when bankrupt to another player (as opposed to the bank)
				player_cash[gaining] += amount;
				//change_color(4);
				printf("%s now has $%d", PlayerNames[gaining], player_cash[gaining]);
				//change_color(0);
				printf("\n");
			}
			for(i = 0; i < 28; i++){
				if(property_ownership[i] == losing){ //property repossession
					property_ownership[i] = gaining;
					printf("%s has repossessed (and is now the owner) of %s\n", (gaining >= 0 ? PlayerNames[gaining] : "The bank"), prop_names[i]);
				   
				   if(i<=21)
				   {
				   	monopolyCheck(i);
				   }
					
				}
			}
			for(i = 0; i < 2; i++){ //goojf card repossession
				if(goojf_ownership[i] == losing){
					goojf_ownership[i] = gaining;
					flag++;
				}
			}
			if(flag > 1)
			{
			  printf("%s has repossessed %s's Get Out of Jail Free Card%s\n", (gaining >= 0 ? PlayerNames[gaining] : "The bank"), PlayerNames[losing], (flag > 2 ? "s" : ""));	
			}
	      getchar();
	      }
		}
	}

	if(!flag && gaining != -1){
		player_cash[gaining] += amount;
		//change_color(4);
		printf("%s now has $%d", PlayerNames[gaining], player_cash[gaining]);
		//change_color(0);
		printf("\n");
	}
    
	return;
}

void ToJail(int player){
	jail_status[player] = 1;
	jail_roll_count[player] = 0; //reset number of attempts to roll out of jail (max 3 rolls before paying)
	player_space[player] = 10; //player put in jail, player sent to the jail space
	RollDIce_over = 1; //dice roll is immediately over
	printf("%s is now in JAIL\n", PlayerNames[player]);
}

void build_house(int player){
	//printf("WIP\n");

	int flag; //used to ensure the player has at least one monopoly
	char group_name[][13] = {"PURPLE/BROWN", "LIGHT BLUE", "PINK", "ORANGE", "RED", "YELLOW", "GREEN", "DARK BLUE"};
	int group_color[] = {9, 12, 10, 5, 1, 6, 3, 11}; //what color to print for each group
	
	int valid_build[3]; //used to ensure that you are not ahead in building in any one property
	int option; //regular option
	int group_option; //player input, necessary to remember since it relates to the group in question
	int group_prop_count; //2 or 3
	int starting_prop; //first property in the group
	int valid_option[6]; //make it easier to determine if a selected option is valid
	int prop; //later used to go through prop_names
	int *p; //used to go through monopoly statuses
	int *h; //used later for going through house counts
	int i;

	while(1){
		i = 0;
		p = &monopoly_status[0];
		flag = 0;
		while(i < 8){
			if(*p && player == property_ownership[GroupStaringProp[i]]){ //ensure there is a monopoly owned by the player before listing the option
				if(!flag) printf("What Group do you want to Build/Sell in?\n");
				flag++;
				//change_color(group_color[i]);
				printf("%d) Build/Sell Houses in the %s group", i+1, group_name[i]);
				//change_color(0);
				printf("\n");
			}
			i++;
			p++;
		}
		if(!flag){ //the player does not have any monopolies: inform them and send them back
			printf("You do not have any monopolies and cannot build or sell any houses yet\n\n");
			return;
		}

		printf("9) Done\n");
		fflush(stdout);
		scanf("%d", &group_option);
		printf("\n");

		if(group_option >= 1 && group_option <= 8){
			starting_prop = GroupStaringProp[group_option-1];
			group_prop_count = (group_option == 1 || group_option == 8 ? 2 : 3);
		}
		else return;

		if(!monopoly_status[group_option-1] || property_ownership[starting_prop] != player) return; //simulate if a plyaer selects an group_option that is invalid
		else{
			while(1){
				for(i = 0; i < 6; i++){ //reset valid options and valid builds
					valid_option[i] = 0; 
					if(i < 3) valid_build[i] = 1; //only 3 indeces for valid_build, by default are all valid
				}

				printf("%s, you have $%d\n", PlayerNames[player], player_cash[player]);
				printf("What property do you want to build or sell a house on?\n");
				prop = starting_prop;
				h = &house_count[starting_prop];
				if(group_prop_count == 2){ //checking for properties with more houses in size 2 groups
					if(*h > *(h+1)) valid_build[0] = 0; //prop 1 has more houses
					else if(*h < *(h+1)) valid_build[1] = 0; //prop 2 has more houses
				}
				else{ //checking for properties with more houses in size 3 groups
					if(*h > *(h+1) || *h > *(h+2)) valid_build[0] = 0;
					if(*(h+1) > *h || *(h+1) > *(h+2)) valid_build[1] = 0;
					if(*(h+2) > *h || *(h+2) > *(h+1)) valid_build[2] = 0;
				}
				for(i = 1; i <= group_prop_count*2; i+=2){
					if(*h < 5 && valid_build[(i-1)/2]){ //make sure there isn't already a hotel (5 houses) and that the property doesn't have more houses (equal building rule)
						printf("%d) ", i); 
						//change_color(5);
						printf("Build ");
						//change_color(0);
						printf("a %s on ", (*h < 4 ? "House" : "Hotel"));
						//change_color(5);
						printf("%s ", prop_names[prop]); 
						//change_color(0); //color the words Build/Sell and the property name yellow/gold
						printf("(");
						//change_color(3);
						printf("currently ");
						//change_color(4);
						printf("%d ", *h);
						//change_color(3);
						printf("house%s", (*h != 1 ? "s" : ""));
						//change_color(0);
						printf(", houses cost $%d)\n", house_costs[group_option-1]); 
						valid_option[i-1] = 1;
					}
					if(*h > 0){ //make sure there is at least one house to sell
						printf("%d) ", i+1);
						//change_color(5);
						printf("Sell "); 
						//change_color(0);
						printf("a House on ");
						//change_color(5);
						printf("%s ", prop_names[prop]); 
						//change_color(0);
						printf("(");
						if(*h == 5){
							//change_color(2);
							printf("currently a hotel"); 
							//change_color(0);
							printf(", ");
						}
						else{
							//change_color(3);
							printf("currently "); 
							//change_color(4);
							printf("%d ", *h);
							//change_color(3);
							printf("house%s", (*h != 1 ? "s" : ""));
							//change_color(0);
							printf(", ");
						}
						printf("houses sell for $%d)\n", house_costs[group_option-1]/2);
						valid_option[i] = 1;
					}
					prop++;
					h++;
				}
				printf("9) Go Back\n");
				fflush(stdout);
				scanf("%d", &option);
				printf("\n");

				if(option >= 1 && option <= 6 && valid_option[option-1]){
					prop = starting_prop;
					h = &house_count[starting_prop];
					prop+=(option-1)/2; //determine where to point (to the correct property)
					h+=(option-1)/2; //determine where to point (to the correct house count)
					printf("\n");
					switch(option){
						case 1: //player bought a house
						case 3:
						case 5:
							if(player_cash[player] < house_costs[group_option-1])
							{
								printf("You donot have enough money to buy this house\n");
								break;
							}
							processPayments(-1, player, house_costs[group_option-1]);
							house_count[starting_prop+((option-1)/2)]++;
							printf("%s bought a house on %s. There ", PlayerNames[player], prop_names[prop]);
							if(*h == 5) printf("is now a hotel ");
							else printf("%s now %d house%s ", (*h != 1 ? "are" : "is"), *h, (*h != 1 ? "s" : ""));
							printf("on %s\n\n", prop_names[prop]);
							break;

						case 2: //player sold a house
						case 4:
						case 6:
							processPayments(player, -1, house_costs[group_option-1]/2);
							house_count[starting_prop+((option-1)/2)]--;
							printf("%s sold a house on %s. There %s now %d houses on %s\n\n", PlayerNames[player], prop_names[prop], (*h != 1 ? "are" : "is"), *h, prop_names[prop]);
							break;

						default:
							printf("ERROR: Shouldn't get here\n");

					}
				}
				else break;
			}
		}
	}
	return;
}

void trade(int player){
  //railroad check krle bhai
	int i,count=0,reciever=-1,recieverproperty=-1,playerproperty=-1;
	int owned[28];
	for(i=0;i<28;i++)
	{
		if(property_ownership[i]== player)
		{
			count++;
			owned[i]=1;
		}
		else
		{
			owned[i]=0;
		}
	}
	if(count==0)
	{
		printf("You dont own any properties to trade \n");
	}
	else
	{ 
	    int input=0;
	    printf("Select the property you want to trade by entering corresponding number \n");
	    int index=0;
		for(i=0;i<28;i++)
		{
			if(owned[i]==1)
			{
			  	printf("%d) %s \n",index+1,prop_names[i]);
			  	index++;
			}
		}
		printf("%d) Go Back \n",index+1);
		printf("\n");
		do 
		{
		  printf("Enter your choice\n");
		  scanf("%d",&input);	
		  if(input <=0 || input > index +1)
		  {
		    printf("Invalid choice enter again\n");	
		  }
		}while(input<=0 || input > index+1);
		
		if(input > 0 && input <= index+1)
		{
			if(input == index + 1)
			{
				return;
			}
			else
			{
				int c=0;
				for(i=0;i<28;i++)
				{
					if(owned[i]==1)
					{
						c++;
					}
					if(c==input)
					{
						playerproperty=i;
						break;
					}
				}
				int z,choice;
				printf("Select a player you want to trade with \n");
				for(z=0;z<player_count;z++)
				{
					if(player_cash[player] != -1)
					{
					  printf("%d) %s \n",z+1,PlayerNames[z]);	
					}
					
				}
				printf("\n");
				do 
				{
					printf("Enter your choie\n");
				    scanf("%d",&choice);
					if(choice <=0 || choice > player_count)	
					{
						printf("Invalid choice enter again\n");	
					}
				}while(choice <=0 || choice > player_count);
	
				if(choice > 0 && choice <= player_count)
				{
					reciever = choice-1;
					int count=0;
				  printf("\n");
				  printf("Select a property of %s you want to trade \n",PlayerNames[choice-1]);
				  for(i=0;i<28;i++)
				  {
				  	if(property_ownership[i]==(reciever))
					  {
					  	printf("%d) %s \n",count+1,prop_names[i]);
					  	count++;
					  }
				  }
				  if(count==0)
				  {
				  	printf("%s dont have any property to trade \n",PlayerNames[choice-1]);
				  }
				  else
				  {
				  	int inp;
				  	do 
					  {
					  	printf("Enter a choice\n");
					    scanf("%d",&inp);  	
					    if(inp<=0 || inp > count)
						{
						  printf("Invalid..enter again\n");	
						}
						
					  }while (inp<=0 || inp > count);
				  	
				  	if(inp > 0 && inp <=count)
					  {
					  	int c=0;
					  	for(i=0;i<28;i++)
						  {
						  	if(property_ownership[i]==reciever)
							  {
							  	c++;
							  }
							  if(c==inp)
							  {
							  	recieverproperty =i;
							  	break;
							  }
						  }
						  printf("TRADE OFFER!!!!\n");
						  printf("%s want to trade %s with %s owned by %s \n",PlayerNames[player],prop_names[playerproperty],prop_names[recieverproperty],PlayerNames[reciever]);
						  printf("%s ! Do you accept this trade press 'Y' to accept or press any other key to reject %s's offer\n",PlayerNames[reciever],PlayerNames[player]);
						  char accept;
						  fflush(stdin);
						  scanf("%c",&accept);
						  if(accept == 'y' || accept == 'Y')
						  {
						  	property_ownership[playerproperty] = reciever;
						  	property_ownership[recieverproperty] = player;
						  	printf("%s now owns %s\n",PlayerNames[player],prop_names[recieverproperty]);
						  	printf("%s now owns %s\n",PlayerNames[reciever],prop_names[playerproperty]);
				            if(playerproperty <=21 )
							{
								monopolyCheck(playerproperty);
							}
							if(recieverproperty <=21)
							{
							  monopolyCheck(recieverproperty);	
							}
						  	
						  	
						  	
						  }
					  }
					  else if(inp == count+1)
					  {
					  	printf("OFFER REJECTED !!!!\n");
					  	return;
					  }
					  else
					  {
					  	printf("invalid choice...input again  ");
					  }
				  	
				  }
				}
				else
				{
					printf("Invalid choice....input again");
				}
			}
		}
		else
		{
			printf("Invalid choice ... input again ");
			return ;
		}
		
		
		
	}
	
	
	

	return;
}

int utilityRent(int factor, int dice_val){ //dice_val will typically be the dice_roll_value global

	return (factor * dice_val);
}

void chance(int player){ 
	//printf("WIP");

	// Chance Guide //
	// 00 = Go Back 3 Spaces
	// 01 = Advance to Illinois Avenue
	// 02 = Pay Poor Tax of $15
	// 03 = Take a Ride on the Reading
	// 04 = Get out of Jail Free Card
	// 05 = Bank Pays You Dividend of $50
	// 06 = Advance to Go (Collect $200)
	// 07 = Take a Walk of the Board Walk Advance Token to Board Walk
	// 08 = Your Building and Loan Matures Collect $150
	// 09 = Advance to St. Charles Place If you pass GO, Collect $200
	// 10 = Go Directly to Jail...
	// 11 = You have been Elected Chairman of the Board Pay Each Player $50
	// 12 = Advance Token to Nearest Utility...
	// 13 = Make General Repairs...
	// 14 = Advance token to the nearest railroad...
	// 15 = Advance token to the nearest railroad...
	
	int card =  chance_deck[chance_remaining_cards];
	int total = 0; //for use in general repairs
	int j;
	int i;

	chance_remaining_cards--;
	if(chance_remaining_cards < 0){
		shuffleChance();
		chance_remaining_cards = 15;
	}

	//change_color(5); //change card text color to yellow
	switch(card){ 
		case 0:
			printf("\"Go Back 3 Spaces\"\n");
			//change_color(0);
			player_space[player] -= 3;
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			break;

		case 1:
			printf("\"Advance to Illinois Ave.\"\n");
			//change_color(0);
			player_space[player] = 24;
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			propertyFunctions(player,0);
			break;

		case 2:
			printf("\"Pay Poor Tax of $15\"\n");
			//change_color(0);
			processPayments(-1, player, 15);
			break;

		case 3:
			printf("\"Take a Ride on the Reading\nIf You pass GO Collect $200\"\n");
			//change_color(0);
			if(player_space[player] > 5){
				printf("%s passed GO! Collect $200\n", PlayerNames[player]);
				processPayments(player, -1, 200);
			}
			player_space[player] = 5;
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			propertyFunctions(player,0);
			break;

		case 4:
			printf("\"This Card may be Kept Until Needed or Sold\nGet Out of Jail Free\"\n");
			//change_color(0);
			if(goojf_ownership[0] == -1) goojf_ownership[0] = player;
			else chance(player);
			break;

		case 5:
			printf("\"Bank Pays You Dividend of $50\"\n");
			//change_color(0);
			processPayments(player, -1, 50);
			break;

		case 6:
			printf("\"Advance to GO\n(Collect $200)\"\n");
			//change_color(0);
			player_space[player] = 0;
			processPayments(player, -1, 200);
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			break;

		case 7:
			printf("\"Take a Walk on the Board Walk\nAdvance Token to Board Walk\"\n");
			//change_color(0);
			player_space[player] = 39;
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			propertyFunctions(player,0);
			break;

		case 8:
			printf("\"Your Building and Loan Matures\nCollect $150\"\n");
			//change_color(0);
			processPayments(player, -1, 150);
			break;

		case 9:
			printf("\"Advance to St. Charles Place\nIf you pass GO, Collect $200\"\n");
			//change_color(0);
			if(player_space[player] > 11){
				printf("%s passed GO! Collect $200\n", PlayerNames[player]);
				processPayments(player, -1, 200);
			}
			player_space[player] = 11;
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			propertyFunctions(player,0);
			break;

		case 10:
			printf("\"Go Directly to Jail\nDo Not Pass Go, Do Not Collect $200\"\n");
			//change_color(0);
			ToJail(player);
			break;

		case 11:
			printf("\"You have been Elected Chairman of the Board\nPay Each Player $50\"\n");
			//change_color(0);
			printf("%s owes %d players $50 (totalling $%d)\n", PlayerNames[player], currentplayers-1, 50 * (currentplayers-1));
			for(i = 0; i < MAX_PLAYERS; i++){
				if(player_cash[i] >= 0){
					if(player != i) processPayments(i, player, 50);
				}
			}
			break;

		case 12:
			printf("\"Advance Token to Nearest Utility.\nIf Unowned you may buy it from bank.\nIf Owned,  pay owner a total  times of dice value thrown\"\n");
			//change_color(0);
			
			if(player_space[player] > 12 && player_space[player] <= 28){
				player_space[player] = 28;
				i = 27; //index of Water Works in Property Ownership
			}
			else{
				player_space[player] = 12;
				i = 26; //index of Electric Company in Property Ownership
			}
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			if(property_ownership[i] == -1) propertyFunctions(player,0);
			else propertyFunctions(player,1);
			break;

		case 13:
			printf("\"Make General Repairs on All Your Property\nFor each House Pay $25\nFor Each Hotel Pay $100\"\n");
			//change_color(0);
			//printf("WIP"); //need to set up houses and such first
			for(i = 0; i < 8; i++){
				if(monopoly_status[i] && property_ownership[GroupStaringProp[i]] == player){ //is there a monopoly, and does the player own it(there can't be any houses if there isn't
					switch(i){
						case 0:
						case 7:
							j = 1; //j+1 properties in the group
							break;

						default:
							j = 2;
					}
					for(j; j >= 0; j--){
						if(house_count[GroupStaringProp[i]+j] < 5) total+=(house_count[GroupStaringProp[i]+j] * 25);
						if(house_count[GroupStaringProp[i]+j] == 5) total+=100;
					}
				}
			}
			if(total){
				printf("%s must pay $%d in repairs\n", PlayerNames[player], total);
				processPayments(-1, player, total);
			}
			else printf("%s does not own any houses or hotels\n", PlayerNames[player]);
			break;


		default:
			printf("\"Advance token to the nearest Railroad and pay owner Twice the Rental to which he is otherwise entitled.\nIf Railroad is unowned, you may buy if from the Bank.\"\n");
			//change_color(0);
			
			while(1){
				player_space[player]++;
				if(player_space[player] % 5 == 0 && player_space[player] % 2 != 0) break;
				if(player_space[player] > 39) 
				{
					player_space[player]=0;
				}
			}
			printf("%s is now on %s\n", PlayerNames[player], space_names[player_space[player]]);
			propertyFunctions(player,1); 
            
		}
	return;
}

void community_chest(int player){
	//printf("WIP");

	// Community Chest Guide //
	// 00 = You Inherit $100
	// 01 = From Sale of Stock You Get $45
	// 02 = You have won Second Prize in a Beauty Contest Collect $10
	// 03 = Get out of Jail Free Card
	// 04 = Bank Error in your Favor Collect $200
	// 05 = Grand Opera Opening Collect $50 from each Player
	// 06 = Receive for Services $25
	// 07 = Go Directly to Jail...
	// 08 = Pay School Tax of $150
	// 09 = You are assessed for street repairs...
	// 10 = Pay Hospital $100
	// 11 = Advance to GO (Collect $200)
	// 12 = Income Tax Refund (Collect $20)
	// 13 = Doctor's Fee Pay $50
	// 14 = XMAS fund Matures Collect $100
	// 15 = Life Insurance Matures Collect $100
	
	int card = cc_deck[cc_remaining_cards];
	int total = 0;
	int j;
	int i;

	cc_remaining_cards--;
	if(cc_remaining_cards < 0){
		shuffleCommunityCards();
		cc_remaining_cards = 15;
	}

	
	switch(card){ 
		case 0:
			printf("\"You Inherit $100\"\n");
			//change_color(0);
			processPayments(player, -1, 100);
			break;

		case 1:
			printf("\"From Sale of Stock You Get $45\"\n");
			//change_color(0);
			processPayments(player, -1, 45);
			break;

		case 2:
			printf("\"You Have Won Second Prize in a Beauty Contest\nCollect $10\"\n");
			//change_color(0);
			processPayments(player, -1, 10);
			break;

		case 3:
			printf("\"Get Out of Jail, Free\nThis Card may be Kept Until Needed or Sold\"\n");
			//change_color(0);
			if(goojf_ownership[1] == -1) goojf_ownership[1] = player;
			else community_chest(player);
			break;

		case 4:
			printf("\"Bank Error in Your Favor\nCollect $200\"\n");
			//change_color(0);
			processPayments(player, -1, 200);
			break;

		case 5:
			printf("\"Grand Opera Opening\nCollect $50 from every Player for Opening Night Seats\"\n");
			//change_color(0);
			printf("%s shall collect $50 from %d players (totalling $%d)\n", PlayerNames[player], currentplayers-1, 50 * (currentplayers-1));
			for(i = 0; i < MAX_PLAYERS; i++){
				if(player_cash[i] >= 0){
					if(player != i) processPayments(player, i, 50);
				}
			}
			break;

		case 6:
			printf("\"Receive for Services $25\"\n");
			//change_color(0);
			processPayments(player, -1, 25);
			break;

		case 7:
			printf("\"Go To Jail\nGo Directly to Jail\nDo Not Pass Go\nDo Not Collect $200\"\n");
			//change_color(0);
			ToJail(player);
			break;

		case 8:
			printf("\"Pay School Tax of $150\"\n");
			//change_color(0);
			processPayments(-1, player, 150);
			break;

		case 9:
			printf("\"You are Assessed for Street Repairs\n$40 Per House\n$115 per Hotel\"\n");
			//change_color(0);
			//printf("WIP"); //need to set up houses and such first
			for(i = 0; i < 8; i++){
				if(monopoly_status[i] && property_ownership[GroupStaringProp[i]] == player){ //is there a monopoly, and does the player own it(there can't be any houses if there isn't
					switch(i){
						case 0:
						case 7:
							j = 1; //j+1 properties in the group
							break;

						default:
							j = 2;
					}
					for(j; j >= 0; j--){
						if(house_count[GroupStaringProp[i]+j] < 5) total+=(house_count[GroupStaringProp[i]+j] * 40);
						if(house_count[GroupStaringProp[i]+j] == 5) total+=115;
					}
				}
			}
			if(total){
				printf("%s must pay $%d in repairs\n", PlayerNames[player], total);
				processPayments(-1, player, total);
			}
			else printf("%s does not own any houses or hotels\n", PlayerNames[player]);
			break;

		case 10:
			printf("\"Pay Hospital $100\"\n");
			//change_color(0);
			processPayments(-1, player, 100);
			break;

		case 11:
			printf("\"Advance to GO\n(Collect $200\")\n");
			//change_color(0);
			player_space[player] = 0;
			processPayments(player, -1, 200);
			printf("%s is now on %s", PlayerNames[player], space_names[player_space[player]]);
			break;

		case 12:
			printf("\"Income Tax Refund\nCollect $20\"\n");
			//change_color(0);
			processPayments(player, -1, 20);
			break;

		case 13:
			printf("\"Doctor's Fee\nPay $50\"\n");
			//change_color(0);
			processPayments(-1, player, 50);
			break;

		case 14:
			printf("\"XMAS Fund Matures\nCollect $100\"\n");
			//change_color(0);
			processPayments(player, -1, 100);
			break;

		default:
			printf("\"Life Insurance Matures\nCollect $100\"\n");
			//change_color(0);
			processPayments(player, -1, 100);
			
	}
	
	return;
}


void sell_property(int player)
{
	int z,count=0,index;
	
	int input;
	for(z=0;z<28;z++)
	{
		if(property_ownership[z] == player)
		{
			count++;
			printf("%d) %s\n",count,prop_names[z]);
		}
	}
	count++;
	printf("%d) Go Back\n",count);
	
	
	do 
	{
	  printf("Select a propert to sell\n");
	  fflush(stdin);
	  scanf("%d",&input);
	  if(input <=0 || input > count)
	  {
	  	printf("invalid choice\n");
	  }
	      	
	}while(input <= 0 || input > count );
	
	if(input == count)
	{
		return;
	}
	else
	{
		int c=0;
		int i;
		for(i=0;i<28;i++)
			{
				if(property_ownership[i]==player)
					{
					  c++;
					}
				if(c==input)
					{
					  index =i;
					  break;
					}
			}
			count=0;
			count++;
			printf("%d)Sell to bank(Bank will buy for half of orignal price)\n",count);
			count++;
			printf("%d)Sell to player for your desired price\n",count);
			fflush(stdin);
			do
			{
				printf("Select either option\n");
				scanf("%d",&input);
				if(input != 1 && input != 2)
				{
					printf("Invalid choice\n");
				}
			}while(input != 1 && input != 2);
			
			if(input == 1)
			{
				property_ownership[index] = -1;
				monopolyCheck(index);
				player_cash[player] += (price[index]/2);
				printf("%s now have %d\n",PlayerNames[player],player_cash[player]);
				return;
			}
			else if(input ==2)
			{
				char choice;
				int price;
				printf("Enter the price for which you want to sell your property\n");
				scanf("%d",&price);
				for(i=0;i<player_count;i++)
				{
					if(player_cash[i] >= 0 && i != player)
					{
						printf("SALE OFFER!!!");
						printf("%s is selling their property %s for $%d\n",PlayerNames[player],prop_names[index],price);
						printf("%s !!!! do you want to buy it? enter Y to accept or any other key to reject\n",PlayerNames[i]);
						fflush(stdin);
						scanf("%c",&choice);
						if(choice == 'y' || choice == 'Y')
						{
							if(player_cash[i] >= price)
							{
								player_cash[i] = player_cash[i] - price;
								player_cash[player] = player_cash[player] + price;
								property_ownership[index]=i;
								monopolyCheck(index);
								printf("%s now have %d\n",PlayerNames[player],player_cash[player]);
								printf("%s now have %d\n",PlayerNames[i],player_cash[i]);
								return;
							}
							else
							{
								printf("You dont have enough money to buy this propert\n");
							}
						}
					}
				}
				
				printf("Property not sold try lowering price next time\n");
			}
			else
			{
				return;
			}
	}
	
	
	
	
	
}


void shuffleChance(){
	int hold;
	int hold_rand;
	int i;

	for(i = 16; i > 0; i--){
		hold_rand = rand() % i;
		hold = chance_deck[i-1];
		chance_deck[i-1] = chance_deck[hold_rand];
		chance_deck[hold_rand] = hold;
	}

	return;
}

void shuffleCommunityCards(){
	int hold;
	int hold_rand;
	int i;

	for(i = 16; i > 0; i--){
		hold_rand = rand() % i;
		hold = cc_deck[i-1];
		cc_deck[i-1] = cc_deck[hold_rand];
		cc_deck[hold_rand] = hold;
	}

	return;
}

int determine_blanks(){//remove
	int biggest_name = 8;
	int i;

	for(i = 0; i < player_count; i++){ //determine longest name
		if(strlen(PlayerNames[i]) > biggest_name) biggest_name = strlen(PlayerNames[i]);
	}

	for(i = 0; i < player_count; i++){ //write spaces to array
		blank_count[i] = biggest_name - strlen(PlayerNames[i]);
	}

	return biggest_name;
}

void printMonopolyBoard() {
    
	char space_namesedited[][24] = {"          GO          ", " Mediterranean Avenue ", "   Community  Chest   ", "    Baltic   Avenue   ", "     Income  Tax     ", "   Reading Railroad   ", "   Oriental  Avenue   ", "        Chance        ", "    Vermont Avenue    ", "  Connecticut Avenue  ", "    Just Visiting!    ", " St .  Charles  Place ", "   Electric Company   ", "    States  Avenue    ", "   Virginia  Avenue   ", "Pennsylvania  Railroad", "   St.  James Place   ", "   Community  Chest   ", "   Tennessee Avenue   ", "  New  York  Avenue   ", "     FREE PARKING     ", "   Kentucky  Avenue   ", "        Chance        ", "    Indiana Avenue    ", "   Illinois  Avenue   ", "    B & O Railroad    ", "   Atlantic  Avenue   ", "    Ventnor Avenue    ", "     Water  Works     ", "    Marvin Gardens    ", "      GO TO JAIL      ", "    Pacific Avenue    ", "North  Carolina Avenue", "   Community  Chest   ", " Pennsylvania  Avenue ", "      Short Line      ", "        Chance        ", "      Park Place      ", "      Luxury Tax      ", "      Board walk      "};//names of spaces or boxes on board	
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("| %s | %s | %s | %s |\n",space_namesedited[0],space_namesedited[1],space_namesedited[2],space_namesedited[3]);
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[39],space_namesedited[4]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[38],space_namesedited[5]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[37],space_namesedited[6]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[36],space_namesedited[7]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[35],space_namesedited[8]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[34],space_namesedited[9]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[33],space_namesedited[10]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[32],space_namesedited[11]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[31],space_namesedited[12]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[30],space_namesedited[13]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[29],space_namesedited[14]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[28],space_namesedited[15]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[27],space_namesedited[16]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[26],space_namesedited[17]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[25],space_namesedited[18]);
    printf("--------------------------                                                 --------------------------\n");
    printf("| %s |                                                 | %s |\n",space_namesedited[24],space_namesedited[19]);
    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("| %s | %s | %s | %s |\n",space_namesedited[23],space_namesedited[22],space_namesedited[21],space_namesedited[20]);
    printf("-----------------------------------------------------------------------------------------------------\n");
    return;
}

