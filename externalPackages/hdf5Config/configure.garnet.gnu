./configure LDFLAGS="-L${PROTEUS_PREFIX}/lib" LIBS="-lsz -lz" CFLAGS="-I${PROTEUS_PREFIX}/include" \
--with-szlib=${PROTEUS_PREFIX} --with-zlib=${PROTEUS_PREFIX} \
--enable-shared --enable-threadsafe -disable-parallel --with-pthread --with-pic \
--prefix=${PROTEUS_PREFIX}

