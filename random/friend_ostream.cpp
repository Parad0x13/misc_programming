// When declared in a class definition this allows you to do stuff like cout << CurrentClass_instance << endl;
friend ostream& operator<<(ostream& stream, CurrentClass& v) {
    stream << "Streaming something custom from an instance of CurrentClass";
    return stream;
};
