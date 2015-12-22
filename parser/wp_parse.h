#pragma pack(1)

typedef struct _AccessObject
{
    USHORT usType;              // Type ob Object (AccessObject = 1)
    USHORT usLen;               // Length of complete Object
    USHORT usClassLen;          // Length of class name
    USHORT usClassOffset;       // Offset of class name from start of AccessObject
    USHORT usTitleLen;          // Length of title 
    USHORT usTitleOffset;       // Offset of title from start of AccessObject
    USHORT usTypeLen;           // Length of type name
    USHORT usTypeOffset;        // Offset of type name from start of AccessObject
    USHORT usPhysNameLen;       // Length of phys name
    USHORT usPhysNameOffset;    // Offset of phys name from start of AccessObject
    USHORT usDefTitleLen;       // Length of default title
    USHORT usDefTitleOffset;    // Offset of default title from start of AccessObject
    ULONG  ulContext_and;       // Context ANDs
    ULONG  ulContext_or;        // Context ORs
    ULONG  ulStyle_and;         // Style ANDs
    ULONG  ulStyle_or;          // Style ORs
    ULONG  ulMisc_and;          // Misc ANDs
    ULONG  ulMisc_or;           // Misc ORs
    CHAR   Strings;             // String-Space (has to be extended)
} AccessObject;

#pragma pack()
