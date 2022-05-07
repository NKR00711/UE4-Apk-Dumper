# UE4-Apk-Dumper
An internal app which can be implemented into game apk and used to generate Sdk Dumps for offsets.

# How to Build?
1. Clone the Repo and Open in Android Studio
2. Compile the App and generate Apk
3. Done

# How to Use?
1. Copy the Library file in the correct Game Libs folder (Shared Libraries) - Currently Supported - 32Bit & 64Bit
2. Rename the dex (dumper apk) to the next number which the game has, For example - If the game has 3 dex file named as classes.dex, classes2.dex & classes3.dex then rename our apk dex as classes4.dex and put inside the game
3. Open MainActivity file of the Game, For Example - BGMI's MainActivity where we can put our code is "SplashActivity" under "onCreate" Function.
4. Now after findind the file put this line - "invoke-static {p0}, Lashope/tech/generator/MainActivity;->CheckOverlayPermission(Landroid/content/Context;)V" under "onCreate" Function. (NOTE - Put this line right before the "onCreate" string inside the "onCreate" Function)
5. Save the Dex file and open our dex file that we copied inside the Game and Delete the "DeleteActivity" file and save the apk
6. Now after all these, Sign the Apk (NOTE - You need to Kill SignVerification Before any of this Tutorial)
7. Open Game and Dump as You Like it.

# How to Get the Dump files?
If you followed the Tutorial on "# How to Use?" properly then a Directory should appear on your sdcard (same place where "Download" folder is present) named as "DumpedGames" and inside should be another folder named as the game Packagename, For Example - For Game BGMI there should be "com.pubg.imobile" folder inside "DumpedGames" folder. Now every Dump should be Present inside their Respective folder.
