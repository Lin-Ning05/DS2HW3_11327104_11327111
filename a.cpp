int Prime(int num) {
    while(true) {
        if (IsPrime(num)) {
            return num;
        }else {
            num++;
        }
    }
}
