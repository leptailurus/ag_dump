# Experiments

## Running DOSBox

```
dosbox -conf examine/local.conf -c "MOUNT C examine/CDrive" -c "MOUNT D examine/experiments/test" -c "C:" -c "CD AGENDA" -c "AGENDA D:\\AFTER.AG" -c "EXIT" 2> /dev/null
```

## Formatted date
```
date +%Y%m%d_%H%M%S
```

## FAT date stamp

* `0x514a`: 1990-10-10 ( 40 `0x28 0010 1000`) ( 10 `0x0a 0000 1010`)
* `0x516e`: 1990-11-14
* `0x6831`: 1993-01-17 ( 52 `0x34 0011 0100`) ( 13 `0x0d 0000 1101`)
* `0x704b`: 1994-02-11 ( 56 `0x38 0011 1000`) ( 14 `0x0e 0000 1110`)
* `0x9167`: 1998-11-07 ( 72 `0x48 0100 1000`) ( 18 `0x12 0001 0010`)
* `0xb13b`: 2002-09-27 ( 88 `0x58 0101 1000`) ( 22 `0x16 0001 0110`)
* `0xd09c`: 2006-04-28 (104 `0x68 0110 1000`) ( 26 `0x1a 0001 1010`)
* `0x1117`: 2014-08-23 (  8 `0x08 0000 1000`) (  2 `0x02 0000 0010`)
* `0x5993`: 2023-12-19 ( 44 `0x2c 0010 1100`) ( 11 `0x0b 0000 1011`)
* `0x6855`: 2025-02-21 ( 52 `0x34 0011 0100`) ( 13 `0x0d 0000 1101`)

Apparently, there is an implementation error in Agenda. Instead of shifting the year number by 9 bits, it is shifted by 11 bits. Thus, considerable range is lost, and the year wraps around every 32 years. Year counter is offset by 1980, so a value of 1980 could also be 2012 or 2044.

## Category search tree

```
L   A  B  C  X   I
01  03 00 00 0  00 Unknown
02  00 0a 05 1  09 C MAIN
03  04 05 01 3  0c C Initial Section
04  0b 06 03 2  10 C Done
05  02 00 03 2  12 C When
06  00 0d 04 1  14 C Entry
0a  00 00 02 0  29 C Standard
0b  00 0c 04 1  2b C Child
0c  00 00 0b 0  2e C Child2
0d  00 00 06 0  30 C Grandchild

A=Left
B=Right
C=Parent

                   04
                   Done
01         03     /
(unknown)- Initial
                  \
                   When
                   05


     0b
     Child
04  /
Done
    \
     Entry
     06


     02
     MAIN
05  /
When
    \


0b   /
Child
     \
      Child2
      0c


06   /
Entry
     \
      Grandchild
      0d


02  /
MAIN
    \
     Standard
     0a

MAIN
  Entry
  When
  Done
  Initial Section
  Standard
    Child
      Grandchild
    Child2

```

## Category observations

* Search node byte 0x15 is 0x01 for category short name
* Category name and short name are separated by semicolon
* Category object 0x2e changed to 0x08 when adding short name - could be length of category name?
* Category object 0x0a changed from 0x04 to 0x00 when adding short name
* Search node: 0x06 next node; 0x08 prev node (double-check)
* !Check how different match expressions affect search nodes
* Category type: 0x34
  * Standard: 0x0100
  * Unindexed: 0x0500
  * Numeric: 0x0900
  * Date/Time: 0x0d00
* Deactivate "Match category name": 0x38: 0x00 -> 0x08
* Deactivate "Match short name": 0x38: 0x00 -> 0x10
* Exclusive children: Category type: 0x01 -> 0x00
* Special actions: Discard
  * 0x26: Pointer to object of type 0x05
* Adv. Text Matching: Global (On) -> Off: 0x39: 0x01 -> 0x41
* Adv. Text Matching: Global (On) -> On: 0x39: 0x01 -> 0x81
* Adv. Match on: Global->Item text: 0x38: 0x00 -> 0x01
  * Note text: 0x02
  * Item&Note: 0x03
* Adv. Req'd match strength Global->x 0x2e: 0x0800 ->
  * Exact 100%: 0x8800
  * Partial 50%: 0x0801
  * Minimal 2%: 0x8801
* Confirm assignments 0x38:
  * Global: 0x0001
  * Always: 0x0003
  * Sometimes: 0x0005
  * Never: 0x0007
* Ignore Suffixes: 0x2e:
  * Global: 0x0800
  * No: 0x0802
  * Yes: 0x0804
* Ignore Accents: 0x2e:
  * Global: 0x0800
  * No: 0x0808
  * Yes: 0x0810
* Assignment Conditions: 0x3a
  * Global: 0x00
  * Off: 0x01
  * On: 0x02
* Assignment Actions: 0x35
  * Global: 0x00
  * Off: 0x04
  * On: 0x08
* Apply conditions: 0x34
  * Automatically: 0x0100
  * On Demand: 0x4101
  * Never: 0x8102
* If Conflict: 0x2e
  * Global: 0x0800
  * Keep old: 0x2800
  * Override old: 0x4800
* Allow explicit assignments: 0x38
  * Yes: 0x0001
  * No: 0x8001
* Make items fit category setting: 0x38
  * Yes: 0x8001
  * No: 0x8000
* Relationship Text/Assignment: 0x38
  * Global: 0x0001
  * OR: 0x2001
  * AND: 0x4001
* Category is protected: 0x2e
  * Global: 0x0800
  * No: 0x0900
  * Yes: 0x0a00
* Can have new children: 0x2e
  * Global: 0x0800
  * No: 0x0820
  * Yes: 0x0840

### Numeric item

### Date categories
* Assign item date: 0x2e
  * Never: 0x0880
  * When item is entered: 0x0882
  * When item text is edited: 0x0884
  * When note text is edited: 0x0886
  * When item or note text is edited: 0x0888
  * When item is assigned: 0x088a
  * When item is assigned or edited: 0x088c
  * From the item text: 0x088e
    * Which one: 0x3c (zero-based)
  * From the note text: 0x0890
  * From the item or note text: 0x0892
* Set the item date from the done key: 0x2e
  * No: 0x0880
  * Yes: 0x8880
* Set the item date from the when key: 0x2e
  * No: 0x0880
  * Yes: 0x0881
* Category can use recurring dates: 0x2e
  * No: 0x0880
  * Yes: 0x08c0


## Special action object
* Type: 0x05
* 0x0c: Pointer to owner category
* 0x12: Text for export file name
* 0x1e:
  * Discard item: 0x00 -> 0x08
  * Designate as done: 0x00 -> 0x10
  * Export to file, discard no: 0x00 -> 0x18
  * Export to file, discard yes: 0x00 -> 0x20


## Preliminary notes on Notes
* Object type: 0x07
* 0x04: Text pointer
* 0x06: Owner pointer
* 0x08: Note file name pointer (Text object)
* 0x0e: Unknown value - Flags? Length?
* Note contents is a chain of text objects
* 0x12: nonzero for notes file


34 d4 21:00