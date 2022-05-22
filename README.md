# Übungsblatt 2
## Allgemeine Hinweise
Für diese und alle folgenden Praktikumsaufgaben gilt, dass Einsendungen, die in der jeweils mitgegebenen Testumgebung nicht laufen, mit null Punkten bewertet werden! Das beinhaltet insbesondere alle Programme, die sich nicht fehlerfrei kompilieren lassen. Als Testsystem werden wir dabei gruenau6 mit den dort installierten Compilern und Compilerwerkzeugen benutzen. Prüfen Sie bitte rechtzeitig vor der Abgabe, ob ihr Programm auch dort lauffähig ist. Wir akzeptieren keine Entschuldigungen der Form „aber bei mir Zuhause hat es funktioniert“ ;).

Ebenfalls mit null Punkten werden alle Abgaben bewertet, die sich nicht exakt an die vorgegebenen Formate halten.

> Um Ihnen die Abgabe zu erleichtern, geben wir Ihnen ein Makefile mit, welches die folgenden Ziele unterstützt:
> #### all
> Übersetzt die Quelldateien und erzeugt eine ausführbare Datei.
> #### run
> Übersetzt die Quelldateien, erzeugt eine ausführbare Datei und startet das Testprogramm.
> #### clean
> Entfernt alle Zwischendateien und räumt in Ihrem Verzeichnis auf.
> Bitte achten Sie bei Ihrer Implementation auf Speicherleckfreiheit bei korrekter Benutzung, d.h. bei paarweisem Aufruf von init() und release().

## Abgabemodus
Ihre Lösung ist in einem öffentlich verfügbaren Git-Repository abzugeben.

Zur Lösung der Aufgaben steht für Sie dieses Repository mit
- mit einem vorgegeben Makefile, 
- dem Scanner minako-lexic.l und einer Header-Datei minako.h sowie der Testeingabe beispiel.c-1 

zur Verfügung.

## Aufgabe 1 (50 Punkte)
### Kurzbeschreibung
Implementieren Sie von Hand einen Parser, der die Sprache C(-1) (eine Teilsprache von C1) erkennen kann. Verwenden Sie dazu wahlweise Ihren Scanner aus Praktikumsaufgabe 2 oder den beigelegten Scanner.

### Aufgabenstellung
Nachdem Sie in der letzten Praktikumsaufgabe einen Scanner für die lexikalische Analyse gebaut haben, sollen Sie sich diesmal mit der syntaktischen Analyse beschäftigen. Wie Sie bereits in der Vorlesung gelernt haben, gibt es mehrere Ansätze, einen Parser zu bauen. Insbesondere wird dabei zwischen handgeschriebenen Parsern und (durch Parsergeneratoren) generierten Parsern unterschieden.

In dieser Aufgabe soll ein handgeschriebener Parser nach dem Prinzip des [rekursiven Abstiegs](https://en.wikipedia.org/wiki/Recursive_descent_parser) implementiert werden. Da ein handgeschriebener Parser in der Regel ziemlich umfangreich ist, haben wir uns dazu entschlossen, die Sprache C1 (die ja durch Abrüsten aus C entstanden ist) noch einmal zu vereinfachen, um Ihnen extreme Tipporgien zu ersparen.

Sie finden die Grammatik von C(-1) [hier](https://amor.cms.hu-berlin.de/~kunert/lehre/material/c-1-grammar.php) und nachfolgend:
```C
program             ::= ( functiondefinition )* <EOF>

functiondefinition  ::= type <ID> "(" ")" "{" statementlist "}"
functioncall        ::= <ID> "(" ")"

statementlist       ::= ( block )*
block               ::= "{" statementlist "}"
                      | statement
statement           ::= ifstatement
                      | returnstatement ";"
                      | printf ";"
                      | statassignment ";"
                      | functioncall ";"

ifstatement         ::= <KW_IF> "(" assignment ")" block
returnstatement     ::= <KW_RETURN> ( assignment )?

printf              ::= <KW_PRINTF> "(" assignment ")"
type                ::= <KW_BOOLEAN>
                      | <KW_FLOAT>
                      | <KW_INT>
                      | <KW_VOID>

statassignment      ::= <ID> "=" assignment
assignment          ::= ( ( <ID> "=" assignment ) | expr )
expr                ::= simpexpr ( ( "==" | "!=" | "<=" | ">=" | "<" | ">" ) simpexpr )?
simpexpr            ::= ( "-" )? term ( ( "+" | "-" | "||" ) term )*
term                ::= factor ( ( "*" | "/" | "&&" ) factor )*
factor              ::= <CONST_INT>
                      | <CONST_FLOAT>
                      | <CONST_BOOLEAN>
                      | functioncall
                      | <ID>
                      | "(" assignment ")"
```
Zusätzlich sind folgende Punkte zu beachten:

- wenn man den Parser ohne Kommandozeilenparameter aufruft, soll er von stdin lesen, ansonsten aus der auf der Kommandozeile angegebenen Datei (dabei sollen eventuell auftauchende Dateizugriffsfehler abgefangen werden)
- die Implementation soll in einer Datei namens minako-syntax.c erfolgen
- der Parser gibt im erfolgreichen Fall nichts aus und beendet sich mit dem Rückgabewert 0
- bei einem Parserfehler wird eine Fehlermeldung (beendet durch ein Newline) ausgegeben und das Programm mit einem Rückgabewert != 0 beendet
- wenn sie den Parser auf das mitgelieferte C(-1)-Beispielprogramm beispiel.c-1 ansetzen, sollte er entsprechend nichts ausgeben und sich mit Rückgabewert 0 beenden - sehen Sie bitte diesen Test als Mindestvoraussetzung für eine Abgabe an.

Bitte halten Sie sich des Weiteren an folgende Konventionen:

- Das jeweils aktuelle Token wird in einer Variablen namens currentToken, das darauffolgende Token (Lookahead) in einer Variablen namens nextToken gespeichert
- Implementieren (und benutzen) Sie eine Funktion namens eat(), die das aktuelle Token konsumiert
- Implementieren (und benutzen) Sie eine Funktion namens isTokenAndEat(), die überprüft, ob das ihr übergebene Token gleich dem aktuellen ist. Im Positivfall wird das aktuelle Token konsumiert, im Negativfall ein Fehler ERROR: Syntaxfehler in Zeile (<zeile>) ausgegeben und das Programm mit Exitcode -1 abgebrochen.
- Implementieren (und benutzen) Sie eine Funktion namens isToken(), die überprüft, ob das ihr übergebene Token gleich dem aktuellen ist und das Ergebnis des Vergleiches als int kodiert zurückgibt.

Viel Erfolg bei der Bearbeitung!
