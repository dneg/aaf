#ifdef AAF_LITERAL_AUID
#undef AAF_LITERAL_AUID
#endif

// macro to help parsing AAFMetaDictionary.h
#define AAF_LITERAL_AUID(L, W1, W2, B1, B2, B3, B4, B5, B6, B7, B8) \
cout.setf(ios::uppercase); \
cout << "// {" << hex << setfill('0') \
     << setw(8) << L << "-" \
     << setw(4) << W1 << "-" \
     << setw(4) << W2 << "-" \
     << setw(2) << B1 << setw(2) << B2 << "-" \
     << setw(2) << B3 \
     << setw(2) << B4 \
     << setw(2) << B5 \
     << setw(2) << B6 \
     << setw(2) << B7 \
     << setw(2) << B8 \
     << "}" << endl; \
cout << "{"; \
cout << #L << ", " << #W1 << ", " << #W2 << ", "; \
cout << "{"; \
cout << #B1 ", " #B2 ", " #B3 ", " #B4 ", "; \
cout <<  #B5 ", " #B6 ", " #B7 ", " #B8; \
cout << "};" << endl; \

