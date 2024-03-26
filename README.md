# SchachBot mit einer pseudo legalen MoveGEN, EP und Castling

## Overview

## [Video](https://cloud.uni-konstanz.de/index.php/s/bGnYB8nRB2BW8Ej)

### Grundlegende Informationen zu meinem Schachbots

Mein Schachbot basiert auf dem Mailbox-Verfahren und verfügt über eine PseudoLegale MoveGEN. Das bedeutet, dass der Bot
alle möglichen Züge generieren kann, ohne die Gültigkeit jedes Zugs zu überprüfen. Diese generierten Züge werden dann an
das Board übergeben, welches die Gültigkeit der Züge prüft und die Spielposition aktualisiert.

### Board

Das Prüfen der Gültigkeit eines Zuges funktioniert bei mir über die `SuperPiece`-Methode. Bei dieser Methode wird ein
virtuelles `Piece` auf das Feld des Königs gesetzt und geschaut, ob diese Figur sich selber nur in einer anderen Farbe
schlagen kann. Die `isKingInCheck`-Funktion holt sich nur die Position vom König und führt dann die `isSquareAttacked`
-Funktion aus, welche `SuperPiece` anwendet.
Beim Mailbox-Verfahren beim `Board` wird im Prinzip einfach nur eine Figur in ein Array abgelegt und wieder
herausgenommen. Um mir das Testen zu vereinfachen, habe ich beim `Board` Funktionen wie `readFen` und `getFen`
geschrieben, welche FEN-Notationen einlesen und ausgeben können. `getFen` nutzt eine private Funktion im `Board`
namens `convertToXandY`, um das EP-Square zu übersetzen.
Ich habe Datentypen angelegt, welche z.B. Einstellungen wie das Struct `board_setting` in `board_setting.h`, die
aktuelle "Side to move" (`player`) in `player_type.h`, ein `Move`-Struct in `Move.h`, wo alle vergangenen Spielzüge in
einem Vector (`moves`) angelegt sind, und natürlich das 64-große Array, welches das Board selber repräsentiert und mit
den `Piece`-Klassen (`Piece.h`) gefüllt ist.
Das Board wird standardmäßig mit leeren Pieces initialisiert, und beim Erstellen des Objektes wird `readFen` ausgeführt
mit dem Standard-FEN, um das Board zu füllen.
Ich habe noch zusätzlich eine `isCheckMate`-Funktion, welche alle möglichen Züge austestet und guckt, wenn keine mehr
möglich sind. Eine `tryToMovePiece`-Funktion gibt es natürlich auch, welche einen Move, den man dazu gibt, auf seine
Korrektheit überprüft. Was heißt korrekt? Es wird getestet, ob der Spieler gerade an der Reihe ist, und auch wirklich
das Piece bewegen und nicht außerhalb des Boards agieren und sich selber nicht capturen kann.
Zusätzlich dazu habe ich natürlich eine `makeMove`-Funktion, welche im Prinzip genau dasselbe macht, nur ist das
einzige, was da validiert wird, dass man sich selber nicht im Check setzt. Diese Funktion ist eher für die `MoveGen`
gedacht, da diese nur Pseudo-Legale Moves generiert.
Da jeder Move, der gemacht wird, noch zusätzlich in einem Vector abgespeichert wird, um einen Rollback zu ermöglichen,
habe ich eine passende `popLastMove`-Funktion. Diese setzt im Prinzip einfach nur alles zurück und stellt auch mithilfe
eines History-Vectors, welcher die `board_settings` hält, auch den letzten Stand der Einstellungen wieder her.
Die letzte noch nicht erwähnte Funktion ist die `handleCastlingPermissions`-Funktion, welche in der `makeMove`-Funktion
unter anderem aufgerufen wird. Sie verändert anhand des aktuellen Moves die Castling-Permissions. Wenn sich ein König
bewegt oder ein Rook captured wird oder sich ebenfalls bewegt, verliert der aktuelle Spieler seine Castling-Rechte.
Natürlich gibt es noch eine `parseMove`-Funktion, welche einen Move generiert anhand des Inputs des Spielers, um weiter
mit einem Move zu arbeiten statt dem Input.
Ich habe das ganze Board in eine Klasse gepackt, um Funktionen wie `handleCastlingPermission` nicht von außen aufrufbar
zu machen, da sie nur intern gebrauchbar ist. Alle Datenstrukturen werden dann in einem Paket geschnürt, und die
Funktionen gehören dann immer zum Board. Somit bleibt alles geordnet und man kann einfacher Funktionen finden und
verwenden.
Ich habe außerdem der `Board`-Klasse einen Subscript-Access Operator gegeben, um direkt auf das private gesetzte `board`
zugreifen zu können. Durch solch einen Operator kann ich z.B. mit meinem verwendeten `assert` immer sicher sein, dass
auch korrekt zugegriffen wird.
Zusätzlich außerhalb der Klasse habe ich eine globale Funktion, welche anhand von X- und Y-Koordinaten einen Index für
dem Board errechnet.

### MoveGEN

In meiner MoveGen-Datei `move_gen.cpp` generiere ich, wie bereits erwähnt, nur Pseudo-Legale Moves. Das sind im Prinzip
Moves, die man machen kann, unabhängig davon, ob man sich selbst im Check setzt. Ich generiere diese Moves, indem ich
z.B. für den Rook, Bishop und die Queen jede mögliche Richtung durchgehe, bis ich außerhalb des Boards bin, auf ein
eigenes Piece stoße oder ein gegnerisches Piece capturieren kann. Da sich alle drei
Figuren `getAllPossibleRookMoves()`, `getAllPossibleQueenMoves()``getAllPossibleBishopMoves()`  gleich verhalten, jedoch
nur die Richtungen unterschiedlich sind, habe ich eine Funktion zu einem template gemacht `getAllLinaerMoves()`, um Code
zu sparen. Ich übergebe im Prinzip nur noch den Direction-Array, der aus einem Pair besteht, das x und y repräsentiert,
und das Piece, das sich dort bewegt, um den Move richtig generieren zu können.
Beim Knight, Pawn und King ist das Prinzip der Generierung der Moves ähnlich, jedoch nicht gleich! Der Knight, King und
Pawn darf ich nicht bis zum Ende des Boards gehen, da diese nur bestimmte feste Richtungen gehen können. Beim Pawn wird
z.B. besonders darauf geachtet, dass er auf Rank 2 und 7 jeweils einen doppelten Sprung hat, je nach Farbe, und beim
Erreichen des 1. oder 8. Ranks promotet er zu Pieces, die ich in einem Array ganz oben in der `.h`-Datei vordefiniert
habe. Nach einem doppelten Sprung von einem Pawn setzt mein `Board::makeMove()` automatisch das
EP-Square `Board::board_settings#EP-Sqaure` auf den Index des
Feldes hinter dem Pawn, um ein EP zu ermöglichen. Ist ein EP möglich, prüfe ich, ob ein diagonaler Sprung auf ein
EP-Square geht und füge den EP-Move hinzu. Beim Pawn wird der Direction-Array einfach negiert, wenn der Pawn Schwarz
sein sollte. Wenn er diagonal moved, prüfe ich auch auf ein nicht leeres Feld, falls EP nicht möglich ist, und
überprüfe, ob das Piece die gegnerische Farbe hat.
Beim King muss vor allem auch darauf geachtet werden, dass dieser mögliche Castling-Moves haben kann. Das erreiche ich
vor allem durch das Überprüfen der aktuellen Permissions, die beim Board nach jedem Move angepasst werden, das
Überprüfen, ob der King im Schach ist oder durch ein Feld zieht, das attacked ist, und das Überprüfen, ob alle Felder
dazwischen leer sind. Ansonsten zieht der King, Knight und der Pawn weiter mit einem Direction-Array. Alle Funktionen
nehmen als Argumente ein Square als `std::pair`, ein board, mit dem sie arbeiten können, eine
PseudoLegalMove-Datenstruktur, die ich selber in `move.h` angelegt habe, und eine pieceColor, um sicherzustellen, dass
man auch die Seiten checken kann, ohne die aktuelle Farbe der Figur zu beachten. Bei der PseudoLegalMove-Struktur habe
ich ein Array, das Moves hält und genau 218 (die maximale Anzahl an Moves) groß ist. Ich habe zusätzlich Iteratoren
eingebaut, um mit der Klasse einfach iterieren zu können, und einen Subscript-Operator sowie eine `pushBack`
-Funktion, die Daten aus meinem Array holen und hinzufügen und den Index anpasst. In meiner `push_back` oder `[]`
-Funktion habe ich auch ein assert drin, das das Programm terminiert, sollte man jemals über das Array hinausgehen. Zu
guter Letzt habe ich eine `getAllPseudoLegalMoves`-Funktion, die nun alle Funktionen aufruft und eine
PseudoLegalMoves-Referenz mitgibt. Dadurch wird meine eigene Move-Struktur mit allen möglichen Moves gefüllt. Sie nimmt
noch das Board und eine Spielerfarbe, um auch Spielermoves holen zu können, unabhängig davon, wer dran ist. Es wird im
Prinzip ein PseudoLegalMoves-Objekt erstellt und von jeder Unterfunktion gefüllt. Ich habe alles in einen Namespace
gepackt, um es geordnet in einem Paket zu haben. Ich habe mich absichtlich gegen eine Klasse entschieden, da alles so
oder so public sein müsste und man keine Objekte von der MoveGen erstellen können sollte.

### ChessBot

In meiner `ChessBot`-Klasse handelt es sich im Wesentlichen um den NegaMax-Algorithmus mit Quiescence Search mit einer
Eval-Funktion.
Mein `ChessBot` inkludiert die `MoveGEN`, da er ohne diese nicht arbeiten kann. Als öffentliche Funktion habe
ich `searchBestNextMove()`, die als Referenz ein Board und eine Tiefe nimmt, um zu bestimmen, wie tief der NegaMax
suchen soll.
Ein NegaMax ist im Grunde genommen nur eine andere Art des MiniMax-Algorithmus, bei dem einfach die Werte umgedreht
werden. Bei diesem Algorithmus gehe ich im Wesentlichen einfach alle möglichen Züge durch, die man aktuell machen kann,
und betrachte dann die möglichen Antworten des Gegners. Das wechselt sich ab, und wenn ich die gewünschte Tiefe erreicht
habe, wird das Board evaluiert mit Quiescence Search, und der Zug mit der besten resultierenden Bewertung ist dann der
beste Zug, den man machen kann. Quiescence Search ist im Prinzip genau dasselbe wie NegaMax, nur das hier nur captures
ausgespielt werden. Genutzt wird QSearch um nach dem Erreichen der Tiefe feststellen ob eine Figur geopfert wird. Der
NegaMax würde den King zwar in Schach setzen, jedoch ohne QSearch `quiescenceSearch()` seine Figuren traden oder
eintauschen. QSearch verhindert das indem genau solche Captures nach dem Erreichen der Tiefe durchgegangen werden.
Zusätzlich habe ich eine Alpha-Beta-Beschneidung eingebaut, die dafür sorgt, dass schlechte Züge nicht weiter in die
Tiefe gehen, um die Leistung auf andere Züge zu konzentrieren. Bei der QSearch selber wird nicht auf eine bestimmte
Tiefe gesucht sondern erstmal alle Captures geordnet mit der Funktion `sortAllMoves()`welche mein AllLegalMoves Objekt
implementiert. Sie ordnet alle moves nach dem mvv-lva oder "most valuable victim, least valuable attacker" Prinzip. So
werden nur Captures durchgegangen bis der Beta Wert erreicht ist. Diese besondere Art der static Evaluierung nennt sich
auch
Standing Pat. Mein NegaMax selbst befindet sich in der
privaten `search()`-Funktion, die von außen nicht aufrufbar sein soll. Diese Funktion nimmt das Board, eine Tiefe, einen
Alpha- und Beta-Wert (die anfangs auf -INT_MAX und INT_MAX gesetzt sein sollten) und `ply`, was nichts anderes ist als
die Anzahl der Ebenen, die wir in der Rekursion tief sind. Diese wird nach jedem Durchlauf um eins erhöht, während die
Tiefe um eins verkleinert wird. Die Alpha- und Beta-Werte werden auch bei jedem Aufruf neu gesetzt. Sobald die
gewünschte Tiefe erreicht ist, wird `quiescenceSearch()` aufgerufen, um das Board zu bewerten. Der Root-Zug mit der
höchsten Bewertung wird dann als bestMove gesetzt.
Nun zur `eval()`-Funktion. In dieser Funktion wird im Wesentlichen der Wert einer Position berechnet. Je genauer die
Funktion den Wert berechnet, desto klüger ist der Bot am Ende. Ich nutze hier genau vier Evaluierungsmethoden. Einmal
das MaterialValue, was im Grunde genommen ein statischer Wert ist, den jedes Piece bekommt. Wenn man nur den
MaterialValue betrachtet, wird der Bot immer versuchen, das teuerste Piece zu schlagen. Man addiert alle Pieces einer
Farbe zusammen und rechnet diese nachher zu den Seitenwerten. Die Materialwerte habe ich von chessprogramming.org
bezogen, um bereits gut
abgestimmte Werte zu haben. Diese sind in der `Piece`-Klasse gespeichert und werden mit `getMaterialValue()` ausgegeben.
Als weiteren Teil der Evaluation nutze ich PSQT, was im Grunde genommen nur ein Bonus für eine bestimmte Position einer
Figur auf einem Schachfeld ist. Als Beispiel hat der Bauer als beste Position den 8. oder 1. Rang, je nach Farbe, und
die Dame ist besser in der Mitte als in der Startposition. Hierfür habe ich mir bereits vorhandene Wertetabellen
ebenfalls von chessprogramming.org geholt wo ich beim Indexieren die bits flippen musste bei schwarzen Figuren damit die
indexierung mit den Tabellen wieder stimmt. Diese habe ich als private und statische Werte in `chess_bot.h`
hinzugefügt,
da sie von außen nicht aufrufbar sein sollen. Jede dieser Tabellen repräsentiert mit mg und eg den midgame- und
endgame-Wert von Positionen einer Figur. Der Wert ist natürlich im Endspiel anders. Ich berechne alle Werte, indem ich
das Brett durchgehe, für jedes Piece die passende Tabelle nehme und den Wert auslese und in einem Array speichere, wobei
0 für Weiß und 1 für Schwarz steht, jeweils für mid- und endgame. Dann wird der Wert des Gegners vom aktuellen
Spielerwert abgezogen. Nun kommt Tampared Eval zum Einsatz. Hier wird unter mid und engame unterschieden mit einer
GamePhase.
Die GamePhase wird ebenfalls anhand und durch die Pieces berechnet, die auf dem Board sind. Ist der MidGame-Wert, der
auf dem
GamePhase-Wert initialisiert wird, größer als 24, wird er auf 24 gesetzt. Dadurch kann der endGame Wert nachher nicht
negativ sein.
Danach wird der EndGame-Wert auf 24 minus midGameValue gesetzt. Nun wird der Score mit der Phase multipliziert und durch
24 geteilt. Die 24 setzt sich durch die Figuren auf dem init board zusammen, also die Startposition. Anhand des Wertes
erkennt man captures, wenn der Wert unter 24
ist, oder
promotions, wenn er größer ist.
Nachdem ich PSQT ausgerechnet habe, den Material-Value dazuaddiert und Tampered Eval verwendet habe kommt TEMPO zum
Einsatz. Das ist im Wesentlichen nur ein Bonus für jede
Seite, die gerade am Zug ist. Ich habe mich für +20 entschieden. Es bringt eigentlich nur den Vorteil, dass die aktuelle
Side to Move immer einen besseren Score hat. Letztendlich wird durch QSearch das ganze noch ein bisschen aufgefrischt da
jetzt keine Figuren mehr eingetauscht werden trotz eines guten Wertes.

### ChessGame

Beim `ChessGame` kommt nun alles zusammen. Hier wird mit dem Konstruktor das Board initialisiert. Da der Bot statisch
ist, braucht dieser nicht initialisiert zu werden. Das Board wird mithilfe eines Smart Pointers auf den Heap gelegt. Ich
nutze hier nur noch die Startfunktion, um das Spiel zu starten. Hier wird im Prinzip das Board einmal ausgegeben und mit
einer Schleife auf die Eingabe des Benutzers gewartet.

Mit 'F' kann man einen gültigen FEN-Code einlesen, indem ich die bereits erwähnte `readFen()`-Funktion des Boards
verwendet. Mit 'Undo' wird zweimal der letzte Zug rückgängig gemacht. Warum zweimal? Sobald man selber einen Zug gemacht
hat, reagiert der Bot. Wenn man nur einmal jedoch poppen würde, wäre man nun selbst Schwarz und könnte Züge des Bots
rückgängig machen.

Falls weder 'Undo' noch 'F' verwendet werden, wird der aktuelle Input als Zug versucht zu parsen. Funktioniert das
Ganze, wird `tryToMovePiece()` vom Board aufgerufen und überprüft, ob dieser Zug erfolgreich war. Wenn dies der Fall
war, wird überprüft, ob der Gegner im Schach Matt steht und gegebenenfalls das Spiel beendet. Falls kein Schach Matt
vorliegt, wird der Bot anhand des aktuellen Boards versuchen, den nächsten besten Zug zu berechnen und auszuführen. Erst
dann wird das neue Board ausgegeben. Danach wird noch einmal überprüft, ob der Bot den Spieler in Schachmatt gesetzt hat
und gegebenenfalls das Spiel beendet. Das Ganze läuft dann in einer Endlosschleife, bis einer der beiden gewinnt.

### Input

* Der Move input ist genau gleich geblieben wie in den Aufgaben nur das capture ist optional geworden und wird
  ignoriert `[Piece][Square][Capture (optional)][Move-Square]` oder als Bsp. `Pa2a4`.
* Man kann weiterhin einen Spielzug rückgängig machen mit `undo`.
* Man kann FEN-Notationen einlesen mit `F[FEN]` oder als
  Bsp. `Frnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1`.
* Man kann sich die aktuelle Position als FEN ausgeben lassen mit `f`.

## Programmierkonzepte

### Objektorientierung

* Kapselung von Funktionen und Objekten für interne Nutzung
* Klassen für Board, ChessBot und Pieces zur Strukturierung von Daten und Verhalten
* Verbesserte Kontrolle über Datenzugriff und -modifikation
* Verwendung in `board.h`, `piece.h`, `move.h` und `chess_bot.h`

### Operatoren

* Direkter Zugriff auf Datenstrukturen im Board und PseudoLegalMoves-Objekt
* Gleichheitsoperator für Move-Vergleiche
* Kontrollierter Zugriff und Modifikation von Daten
* Verwendung in `board.h` und `moves.h`

### Iteratoren

* Einfaches Durchlaufen aller Moves in PseudoLegalMoves
* Verbesserte Lesbarkeit und Wartbarkeit des Codes
* Verwendung in `move.h` (PseudoLegalMoves)

### Templates

* Generische MoveGen-Funktion für Rook, Queen und Bishop
* Vermeidung von Codeduplizierung und Reduzierung des Wartungsaufwands
* Verwendung in `move_gen.cpp`

### Lambdas

* Namenlose Funktionen somit keine Namenskollision
* Der Code ist einfacher zu lesen und kann dort definiert werden wo man ihn brauch.
* Verwendung in `move.h#sortAllMoves()`

### Exceptions

* Fehlermeldung beim Einlesen unvollständiger FEN-Notationen
* Vermeidung von Abstürzen und Verbesserung der Benutzerfreundlichkeit
* Verwendung in `board.cpp` (readFen)

### Eigene Datenstrukturen

* Bündelung zusammengehöriger Daten in Objekten (z.B. Moves)
* Verbesserte Organisation und Übersichtlichkeit des Codes
* Verwendung in `move.h`, `board_settings.h` und `player.h`

### Asserts

* Prüfung auf Array-Grenzüberschreitungen und andere illegale Operationen
* Früherkennung von Fehlern und Programmabstürzen
* Verbesserte Debugging-Möglichkeiten
* Verwendung in `board.h` und `move.h`

### Namespaces

* Bessere Code-Organisation durch die Gruppierung von Funktionen.
* Wiedererkennbarkeit von Funktionen einer Gruppe.
* Verwendung in `move_gen.cpp`

## Libraries

### Google Test Bibliothek

In diesem Projekt verwende ich die Google Test Bibliothek ([gtest](https://github.com/google/googletest)) für
automatisierte Tests.

#### Warum gtest?

- Bekanntheit: gtest ist mir aus den Vorlesungen vertraut und daher bereits bekannt.
- Einfachheit: Die Bibliothek ist einfach zu bedienen und übersichtliche DOCs, was das Verwenden erleichtert.
- Effizienz: gtest ermöglicht die effiziente Durchführung von Unit-Tests und die schnelle Fehlerbehebung.

#### Wo wird gtest eingesetzt?

Tests im /test Verzeichnis: Hier schreibe ich meine Tests, die nach jeder Änderung im Code ausgeführt werden sollten um
sicher zu stellen das alles Funktioniert.

#### Vorteile von gtest:

- Schnellere Entwicklung: Durch automatisierte Tests kann die Entwicklungszeit verkürzt und die Fehlerrate reduziert
  werden.
- Höhere Codequalität: gtest fördert die Testabdeckung und hilft dabei, saubereren und stabileren Code zu schreiben.
- Verbesserte Wartbarkeit: Tests erleichtern die Wartung und Weiterentwicklung des Codes.

#### Fazit:

gtest ist eine wertvolle Bibliothek für die automatisierte Testerstellung in C++ Projekten. Die Bibliothek ist einfach
und bietet viele Vorteile, die die Entwicklung und Wartung von meiner Engine verbessern.

## Testing

#### Test 1: Move-Generierung und Perft-Validierung

Der erste Test fokussiert sich auf die Move-Generierung der Engine. Mithilfe der Perft-Methode, die alle möglichen Züge
bis zu einer bestimmten Tiefe aufzählt, kann ich die Korrektheit der generierten Züge überprüfen und
Grundfunktionalitäten der Engine werden nebenher mitgetestet.

- **Verwendete TestSuite**: Ethereal Perft Test Suite (https://github.com/AndyGrant/Ethereal)
- **Testfall**: Vergleich der Anzahl der generierten Züge mit den Perft-Referenzwerten
- Zusätzliche Funktionen die nebenher mitgetestet werden:
  * FEN-Einlesen
  * Korrektes Ausführen von Zügen
  * Zurücksetzen von Zügen
  * Schacherkennung
  * Legale Spieler bewegungen
  * Korrekter Zugriff auf das Board.

#### Test 2: Fen ausgabe

Der zweite Test überprüft die ausgabe des FEN. Hier habe ich von der bereits verwendeten Perft-Suite die FEN-Notationen
geholt, eingelesen und mir wieder ausgeben lassen. Dadurch kann man validieren, das bei der Ausgabe auch wirklich der
richtige FEN generiert wird.

- **Testfall**: Einlesen des FEN und das Ausgeben des Boards als FEN
- **Vergleich**: Vergleich des geparsten FENs mit dem eingelesenen FEN

#### Test 3: Move-Parsing und Validierung

Der dritte Test überprüft das Parsing und die Validierung von Spielerzügen. Hier habe ich mit den FEN Codes des PERFT
Tests mir alle korrekten moeglichen Zuege ausgeben lassen und werde diese einlesen und wieder ausgeben lassen. Bleibt
der Zug
gleich, funktioniert das Parsing.

- **Testfall**: Einlesen des FEN und das Parsen und Ausgeben der Züge
- **Vergleich**: Vergleich des geparsten Zugs mit dem Originalzug

#### Test 4: Asserts

Asserts welche immer wieder im Code verteilt auftauchen, prüfen den korrekten Zugriff auf einzelne Sachen wie die
Anzahl der Elemente bei AllPseudoMoves oder bei dem Board.

#### Ergebnisse und Schlussfolgerung:

Mit diesen paar Tests konnte ich die Grundfunktionalität meiner Schachengine erfolgreich auf Korrektheit überprüfen.
Sowohl die Move-Generierung als auch das Parsing, das Zuruecksetzen und Ausfuehren von einem Zug und die Validierung von
Spielerzügen funktionieren fehlerfrei. Da ich bei einem Zug von dem Spieler den geparsten Zug abgleiche mit meiner
MoveGEN ist garantiert das der Spieler nur einen Zug machen kann welcher auch von der MoveGEN generiert werden kann.
Diese ist mit Perft auf ihre Korrektheit ueberprueft und somit kann sich der Spieler nur legal bewegen.

### **Wichtig: Beim Testen den Pfad in /tests/tests.cpp anpassen, da absolute Pfade verwendet werden!**

## Fremdcode und Informationen

### Verwendete Quellen

* [Negamax](https://www.chessprogramming.org/Negamax)
* [Quiescence Search](https://www.chessprogramming.org/Quiescence_Search)
* [Tampered Eval](https://www.chessprogramming.org/Tapered_Eval)
* [PSQT](https://www.chessprogramming.org/Piece-Square_Tables)
* [Material](https://www.chessprogramming.org/Material)
* [Tempo](https://www.chessprogramming.org/Tempo)
* [PeSTO's Eval](https://www.chessprogramming.org/PeSTO's_Evaluation_Function)
* [Ethereal PERFT](https://github.com/AndyGrant/Ethereal/blob/master/src/perft/standard.epd)

### Fremdcode

Es wurden die Werte der PSQT-Arrays und die Material Werte
von [PeSTO](https://www.chessprogramming.org/PeSTO's_Evaluation_Function) übernommen und
implementiert in `chess_bot.h::53-124` und `piece.h::79-80`.
Grund hierfür sind die beliebten Values welche man direkt mitverwenden kann. Da diese Werte alle fein abgestimmt wurden,
erreicht man somit eine bessere evaluierung als diese selbst aufzustellen und somit ein besseren Schach-Bot.

Es wurde ein kleiner Teil der Evaluierung übernommen und ggf. angepasst in `chess_bot.h/eval::81-87`
[Tampered Eval](https://www.chessprogramming.org/Tapered_Eval). Ich habe hier die Berechnung übernommen weil man
so mit Tampered Eval den Wert berechnet und da nicht drumherum
kommt. [PeSTO's Eval](https://www.chessprogramming.org/PeSTO's_Evaluation_Function) Werte wurden hier natürlich auch
verwendet.

Ich habe die PERFT test suit von [Ethereal](https://github.com/AndyGrant/Ethereal/blob/master/src/perft/standard.epd)
uebernommen da diese Daten alle korrekt sind und fuer das testen gut geeignet sind.
