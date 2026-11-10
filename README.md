# Carthage Airways

Application desktop de gestion pour une compagnie aérienne - Projet Programmation C

## Description
Ce projet permet de gérer les passagers, vols, réservations, personnel navigant, avions et aéroports d'une compagnie aérienne fictive.

## Équipe
- [Vos noms ici]

## Technologies
- Langage C
- Interface graphique : Glade/GTK+
- Système : Linux Ubuntu

## Base De Donnees Spec  
### permenant databases  
aerorport database  

### passenger
```  
passport number | nom prenom | age | point de fidelite | state (if its inactive reason in paratheses)
```  

history file between account number and vols id and price of each one  

something like this  
  
```  
account_number | vol id | vol id 2 ...  
```  
### Vols  
```  
start-timeof-vol-sha256(of vol info)(transform it into aski IG) | airline | aeroport 1 | time1 | aeroport 2 | time2:time2 |
```  

another idea  
we are linking vol to planes  
```  
vol-id | plane-id
```  

### Reservation  
```  
idisthelinenumber | sha256(transform it into asci) | state | vol-id | (true or false) for each service on the specific order we specify in the code | 0(g) if its 0 there is no bagage supplementaire
```  
we will show the user the id of reservation as account_number-numberof_line  
  
   
### personel navigant  
2 files  
one navigant_cin_each_country_code  
example: navigant_cin_TN  
 navigant_passport  
 ```
 passport number as id | cin | country_code | ...
 ```    
 if a new account doens't have passport number  
 navigant_cin_TN  
 ```  
 cin as id | ...   
 ```  
when an account adds a passport number we move them to the other file  
an account doesn't have the right to remove passport number  
  
both files share the same other data  
```  
critere ?? | licence | date d'expiration | number_of_hours_in_flight | disponibility percentage ?? | base d'attache ?? | fonction (a number from a defined array)
```      
```  
[pilote, copilote,personel de cabine ...]  
```
linking file  
```  
vol-id | file | cin | cin | cin | file2 | passport number | passport number
```
  
### avions  
```  
plane-id | capacite_max | last checkup date |next checkup date | (number of minutes in flight) convert to hours in interface logic | state
```    
  
```  
plane-id-date-of-maintainance-1/2/3/4 | type | duree | cout
```  
  
### aeroport  
```  
IATA aerorport id | name | horaire | city | country ig | 
```   
  
```  
IATA aeroport id | date0 | number of flights depart/arrive in date0 | date1 etc...
```  
  
### authentication accounts etc  
```  
account id we generate it randomly based on ms since epoch (if somehow they create account in same ms we just give them the next random number) | email address  | password hashed
```