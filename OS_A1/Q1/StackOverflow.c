int infinteRec(long long a);

int infinteRec(long long a) {
    a++;
    return infinteRec(a);
}

int main() {
    long long a = 0;
    infinteRec(a);
    return 0;
}