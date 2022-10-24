#include "elf.h"
#include <stddef.h>

static void *memSet(void *s, int c, size_t n)
{
  if (NULL == s || n < 0)
    return NULL;
  char * tmpS = (char *)s;
  while(n-- > 0)
    *tmpS++ = c;
    return s; 
}

// static void *memCpy(void *dest, const void *src, size_t n)
// {
//   if (NULL == dest || NULL == src || n < 0)
//     return NULL;
//   char *tempDest = (char *)dest;
//   char *tempSrc = (char *)src;
 
//   while (n-- > 0)
//     *tempDest++ = *tempSrc++;
//   return dest;  
// }

  void *memCpy(void *dest, void *src, uint32_t n)
  {
    if (NULL == dest || NULL == src || n < 0)
      return NULL;
    uint32_t *tempDest = (uint32_t *)dest;
    uint32_t *tempSrc = (uint32_t *)src;
    uint32_t i =0;
    for(i = 0; i <= n / 4; i++)   // remain n % 4 memory not set, so i <= n / 4
    {
      tempDest[i] = tempSrc[i];
    }
    return dest;
  }

  int load_elf(const uint8_t *elf, const uint32_t elf_size) {
  // sanity checks
  if(elf_size <= sizeof(Elf32_Ehdr))
    return 1;                   /* too small */

  const Elf32_Ehdr *eh = (const Elf32_Ehdr *)elf;
  if(!IS_ELF32(*eh))
    return 2;                   /* not a elf32 file */

  const Elf32_Phdr *ph = (const Elf32_Phdr *)(elf + eh->e_phoff);
  if(elf_size < eh->e_phoff + eh->e_phnum*sizeof(*ph))
    return 3;                   /* internal damaged */

  uint32_t i;
  for(i=0; i<eh->e_phnum; i++) {
    if(ph[i].p_type == PT_LOAD && ph[i].p_memsz) { /* need to load this physical section */
      printf("[load_elf]still alive ... writing %d bytes to ", ph[i].p_filesz);
      //printf("%x \n\r", (uintptr_t)ph[i].p_paddr);
      printf("%x \n\r", (uint32_t)ph[i].p_paddr);
      
      if(ph[i].p_filesz) {                         /* has data */
        if(elf_size < ph[i].p_offset + ph[i].p_filesz)
          return 3;             /* internal damaged */
        memCpy((uint8_t *)ph[i].p_paddr, elf + ph[i].p_offset, ph[i].p_filesz);
      }
      if(ph[i].p_memsz > ph[i].p_filesz) { /* zero padding */
        memSet((uint8_t *)ph[i].p_paddr + ph[i].p_filesz, 0, ph[i].p_memsz - ph[i].p_filesz);
      }
    }
  }
  return 0;
}

int load_elf_sd(const uint8_t *elf, const uint32_t elf_size) {
  // sanity checks
  if(elf_size <= sizeof(Elf32_Ehdr))
    return 1;                   /* too small */

  const Elf32_Ehdr *eh = (const Elf32_Ehdr *)elf;
  if(!IS_ELF32(*eh))
    return 2;                   /* not a elf32 file */

  const Elf32_Phdr *ph = (const Elf32_Phdr *)(elf + eh->e_phoff);
  if(elf_size < eh->e_phoff + eh->e_phnum*sizeof(*ph))
    return 3;                   /* internal damaged */
  // uint32_t entry_point = eh->e_entry;
  // printf("elf entry point addr : %x \n", entry_point);
  uint32_t i;
  for(i=0; i<eh->e_phnum; i++) {
    if(ph[i].p_type == PT_LOAD && ph[i].p_memsz) { /* need to load this physical section */
      printf("[load_elf]still alive ... writing %d bytes to ", ph[i].p_filesz);
      //printf("%x \n\r", (uintptr_t)ph[i].p_paddr);
      printf("%x \n\r", (uint32_t)ph[i].p_paddr);
      
      if(ph[i].p_filesz) {                         /* has data */
        if(elf_size < ph[i].p_offset + ph[i].p_filesz)
          return 3;             /* internal damaged */
        memCpy((uint8_t *)ph[i].p_paddr, elf + ph[i].p_offset, ph[i].p_filesz);
      }
      if(ph[i].p_memsz > ph[i].p_filesz) { /* zero padding */
        memSet((uint8_t *)ph[i].p_paddr + ph[i].p_filesz, 0, ph[i].p_memsz - ph[i].p_filesz);
      }
    }
  }
  return 0;
}

uint32_t get_entry(const uint8_t *elf, const uint32_t elf_size)
{
    // if(elf_size <= sizeof(Elf32_Ehdr))
    // return 1;                   /* too small */

    const Elf32_Ehdr *eh = (const Elf32_Ehdr *)elf;
    // if(!IS_ELF32(*eh))
    //   return 2;                   /* not a elf32 file */

    const Elf32_Phdr *ph = (const Elf32_Phdr *)(elf + eh->e_phoff);
    // if(elf_size < eh->e_phoff + eh->e_phnum*sizeof(*ph))
    //   return 3;                   /* internal damaged */
    return  eh->e_entry;
}

