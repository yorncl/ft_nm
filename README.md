# ft_nm

A simple implementation of the utility `nm` for objects, shared objects in x86 and x86_64.
It should mimic its output, it should not manage any options flags (as of now).
It assumes a somewhat well formed ELF file (it would probably break with duplicate system section names like .strtab, the spec isn't clear about the intended behaviour so I ignored this case).

To build `ft_nm`, run `make`.

To launch the tests hit `make test`.

