#ifdef AAF_LITERAL_AUID
#undef AAF_LITERAL_AUID
#endif

// macro to help parsing AAFMetaDictionary.h
#define AAF_LITERAL_AUID(L, W1, W2, B1, B2, B3, B4, B5, B6, B7, B8) \
cout << "{"; \
cout << #L << ", " << #W1 << ", " << #W2 << ", "; \
cout << "{"; \
cout << #B1 ", " #B2 ", " #B3 ", " #B4 ", "; \
cout <<  #B5 ", " #B6 ", " #B7 ", " #B8; \
cout << "};" << endl; \

