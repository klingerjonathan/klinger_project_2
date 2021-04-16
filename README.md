# klinger_project_2
> Zweites NVS - Projekt 2021
> Klinger Jonathan 5BHIF

## Angabe lt. PDF
Funktionsfähiger HTTP 1.1‐Client zum Herunterladen/Speichern von
3
beliebigen Dateien (GET, PUT, POST, DELETE, Basisfunktionalität;
Steuerung über Kommandozeile; inkl. HTTP Basic‐Authentication,
nicht auf Header vergessen und inkl. Cookies, http‐parser kann verwendet werden)

## Verwendung
Das Programm ermöglicht es 3 HTTP Request (GET, POST, PUT oder DELETE) an 3 verschiedene Server zu senden. Die jeweiligen informationen müssen in einer vorgesehen Reihenfolge angegeben werden. Basic Authentication ist möglich. 

Ein GET Aufruf: 
```
./klinger_project_2 -1 GET abc.xyz /abc [cookies] test.txt [username passwort]
```

Ein POST Aufruf:
```
./klinger_project_2 -1 POST abc.xyz /abc [cookies] content-type content [username passwort]
```

Ein schneller Test kann mit dieser Test Website durchgeführt werden
```
./klinger_project_2 -1 GET ptsv2.com 80 /t/jonny/post test.txt  
```

Hier ein Test mit Cookie:
```
./klinger_project_2 -1 GET ptsv2.com 80 /t/jonny/post moin=meister test.txt   
```

Jeder aufruf startet mit der Zahl des requests. Also -1 für den Ersten -2 für den Zweiten usw. 
Danach folgt die URL des Zielservers. Es *muss* ein weiterer Pfad angegeben werden als nächstes (zumindest ein /). Danach folgt der Dateiname in den das Ergebnis gespeichert werden soll und gegebenenfalls Username und Passwort für eine Authentifizierung. 

## Hinweise 
Leider habe ich die letzten Commits nach 23:59 getätigt war aber dennoch Bemüht rechtzeitig Fertig zu werden. Ich denke ich habe die Aufgabe (auch nach Rückfrage per E-Mail) einfach aber passend gelöst und bin trotz mehrerer Rückschläge fertig geworden. 

Außerdem sind bei mir jegliche Versuche httplib und spdlog auch mit der meson_option.txt einzubinden auch nach rücksprache mit Kollegen nicht funktioniert weshalb sie in /include zu finden sind. :/ 

Anm.: Habe Vergessen die Ausarbeitung rechtzeitig ins repo hinzuzufügen. Bitte um Nachsicht!


MfG
Jonathan Klinger