# FW_cambio_indirizzo_i2c_programmatore_flexa

**HARDWARE**
- "Programmatore"/"Convertitore" di indirizzi i2c del sensore di temperatura MLX90614, montato e consegnato a Flexa (utilizzato da Flexa per cambiare l'indirizzo del sensore di temperatura a seconda del rullo su cui verrà posizionato, '0x5A' per il sensore del rullo superiore e '0x5B' per il sensore del rullo inferiore)
- Scheda Arduino Mega
- Shield con connettore sensore, connettore led e connettore interruttore montata su scheda Arduino Mega
- Tra il connettore sensore e il microcontrollore è stato montato l'i2c extender e tra il connettore led e il microcontrollore sono state montate due resistenza da 1K1 (tutto posizionato sotto la shield, ricoperto da colla a caldo), anche il led d'alimentazione ha una resistenza da 1K1
- Alimentazione 220V AC (collegare alimentatore ad una presa), il "jack" d'alimentazione porterà 12V DC alla scheda Arduino Mega se l'interruttore d'alimentazione posto sul case del "convertitore" verrà girato (controllare l'avvenuta accensione se il led rosso è acceso)

**AMBIENTE DI SVILUPPO**
- Arduino IDE
- Ambiente e librerie di Arduino

**LIBRERIE**
- Wire : libreria presente all'interno dell'ambiente Arduino per utilizzare l'i2c
- SparkFun_MLX90614 : libreria che gestisce il sensore di temperatura MLX90614

**FUNZIONAMENTO IN SINTESI**
- Il programma continuerà in loop ad effettuare una scansione del bus i2c e accenderà il led a cui corrisponde l'indirizzo i2c trovato con la scansione. Quando verrà premuto l'interruttore, verrà cambiato l'indirizzo i2c al sensore di temperatura collegato al "convertitore" (se l'indirizzo attuale del sensore è '0x5A', verrà cambiato in '0x5B', o viceversa) e successivamente verrà riavviato il sensore per applicare il cambio indirizzo i2c. Il programma continuerà ad effettura la scansione del bus i2c e rimarrà in attesa di una nuova interazione dall'interruttore.

**FUNZIONAMENTO CODICE**
- scan() : funzione che scannerizza il bus i2c e accende il led a cui corrisponde l'indirizzo i2c del sensore collegato al "convertitore"

- change() : funzione che cambia indirizzo i2c al sensore di temperatura e lo riavvia per applicare il cambio d'indirizzo

- setup() : inizializza seriale, i2c e i 3 pin che verranno utilizzati dal programma (2 led e 1 interruttore)

- loop() : esegue scansione bus i2c in continuazione e se viene premuto l'interruttore, verrà eseguita la funzione "change()" e successivamente il programma continuerà con la scansione
