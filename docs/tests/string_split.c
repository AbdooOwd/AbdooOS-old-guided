size_t split(char str[], char splitter, char* splitten[]) {
    char* scanned;
    int i = 0, j = 0;

    // go through each char of str
    for (i; i < strlen(str); i++) {
        if (str[i] == splitter) {
            // if it's splitter then we add a new element
            // and we j+1 to go to next bababoey

            // null-terminate the string
            scanned[strlen(scanned)] = '\0';
            splitten[j] = scanned;          // add the string
            j++;                            // next index
            scanned[0] = '\0';              // Clear the string

            print(splitten[j]);
        }
        else {
            append(scanned, str[i]);
        }
    }

    scanned[strlen(scanned)] = '\0';
    splitten[j + 1] = scanned;

    return j + 1; // add last elem
}