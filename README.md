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

Ein Beispielaufruf für 2 Requests:
```
./klinger_project_2 --type1 GET --url1 ptsv2.com --path1 /t/jonny/post --port1 80 
--cookies1 cookie=value --file1 test1.txt --user1 user --pw1 password --type2 POST 
--url2 ptsv2.com --path2 /t/jonny/post --port2 80 --contentType2 text/plain 
--content2 Content ... 
```

Jeder aufruf startet mit der Zahl des requests. Also -1 für den Ersten -2 für den Zweiten usw. 
Danach folgt die URL des Zielservers. Es *muss* ein weiterer Pfad angegeben werden als nächstes (zumindest ein /). Danach folgt der Dateiname in den das Ergebnis gespeichert werden soll und gegebenenfalls Username und Passwort für eine Authentifizierung. 

## Hinweise 
Leider habe ich die letzten Commits nach 23:59 getätigt war aber dennoch Bemüht rechtzeitig Fertig zu werden. Ich denke ich habe die Aufgabe (auch nach Rückfrage per E-Mail) einfach aber passend gelöst und bin trotz mehrerer Rückschläge fertig geworden. 

Außerdem sind bei mir jegliche Versuche httplib und spdlog auch mit der meson_option.txt einzubinden auch nach rücksprache mit Kollegen nicht funktioniert weshalb sie in /include zu finden sind. :/ 

Anm.: Habe Vergessen die Ausarbeitung rechtzeitig ins repo hinzuzufügen. Bitte um Nachsicht!


MfG
Jonathan Klinger