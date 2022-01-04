" Vim syntax file
" Language:     Asymptote
" Maintainer:   Andy Hammerlindl
" Last Change:  2005 Aug 23

" Hacked together from Bram Moolenaar's C syntax file, and Claudio Fleiner's
" Java syntax file.

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syn clear
elseif exists("b:current_syntax")
  finish
endif

" useful C/C++/Java keywords
syn keyword     asyStatement     break return continue unravel
syn keyword     asyConditional   if else
syn keyword     asyRepeat        while for do
syn keyword     asyExternal      access from import include
syn keyword     asyOperator      new operator

" basic asymptote keywords
syn keyword     asyConstant      VERSION
syn keyword     asyConstant      true false default infinity inf nan
syn keyword     asyConstant      null nullframe nullpath nullpen
syn keyword     asyConstant      intMin intMax realMin realMax
syn keyword     asyConstant      realEpsilon realDigits
syn keyword     asyPathSpec      and cycle controls tension atleast curl
syn keyword     asyStorageClass  static public restricted private explicit
syn keyword     asyStructure     struct typedef
syn keyword     asyType          void bool bool3 int real string file
syn keyword     asyType          pair triple transform guide path pen frame
syn keyword     asyType          picture

" module specific keywords
if exists("asy_syn_plain")
  syn keyword   asyConstant      currentpicture currentpen defaultpen
  syn keyword   asyConstant      inch inches cm mm bp pt up down right left
  syn keyword   asyConstant      E NE N NW W SW S SE
  syn keyword   asyConstant      ENE NNE NNW WNW WSW SSW SSE ESE
  syn keyword   asyConstant      I pi twopi
  syn keyword   asyConstant      CCW CW
  syn keyword   asyConstant      undefined sqrtEpsilon Align mantissaBits
  syn keyword   asyConstant      identity zeroTransform invert
  syn keyword   asyConstant      stdin stdout
  syn keyword   asyConstant      unitsquare unitcircle circleprecision
  syn keyword   asyConstant      solid dotted Dotted dashed dashdotted
  syn keyword   asyConstant      longdashed longdashdotted
  syn keyword   asyConstant      squarecap roundcap extendcap
  syn keyword   asyConstant      miterjoin roundjoin beveljoin
  syn keyword   asyConstant      zerowinding evenodd basealign nobasealign
  syn keyword   asyConstant      black white gray red green blue Cyan Magenta
  syn keyword   asyConstant      Yellow Black cyan magenta yellow palered
  syn keyword   asyConstant      palegreen paleblue palecyan palemagenta
  syn keyword   asyConstant      paleyellow palegray lightred lightgreen
  syn keyword   asyConstant      lightblue lightcyan lightmagenta lightyellow
  syn keyword   asyConstant      lightgray mediumred mediumgreen mediumblue
  syn keyword   asyConstant      mediumcyan mediummagenta mediumyellow
  syn keyword   asyConstant      mediumgray heavyred heavygreen heavyblue
  syn keyword   asyConstant      heavycyan heavymagenta lightolive heavygray
  syn keyword   asyConstant      deepred deepgreen deepblue deepcyan
  syn keyword   asyConstant      deepmagenta deepyellow deepgray darkred
  syn keyword   asyConstant      darkgreen darkblue darkcyan darkmagenta
  syn keyword   asyConstant      darkolive darkgray orange fuchsia chartreuse
  syn keyword   asyConstant      springgreen purple royalblue salmon brown
  syn keyword   asyConstant      olive darkbrown pink palegrey lightgrey
  syn keyword   asyConstant      mediumgrey grey heavygrey deepgrey darkgrey

  if exists("asy_syn_texcolors")
    syn keyword asyConstant      GreenYellow Yellow Goldenrod Dandelion
    syn keyword asyConstant      Apricot Peach Melon YellowOrange Orange
    syn keyword asyConstant      BurntOrange Bittersweet RedOrange Mahogany
    syn keyword asyConstant      Maroon BrickRed Red OrangeRed RubineRed
    syn keyword asyConstant      WildStrawberry Salmon CarnationPink Magenta
    syn keyword asyConstant      VioletRed Rhodamine Mulberry RedViolet
    syn keyword asyConstant      Fuchsia Lavender Thistle Orchid DarkOrchid
    syn keyword asyConstant      Purple Plum Violet RoyalPurple BlueViolet
    syn keyword asyConstant      Periwinkle CadetBlue CornflowerBlue
    syn keyword asyConstant      MidnightBlue NavyBlue RoyalBlue Blue
    syn keyword asyConstant      Cerulean Cyan ProcessBlue SkyBlue Turquoise
    syn keyword asyConstant      TealBlue Aquamarine BlueGreen Emerald
    syn keyword asyConstant      JungleGreen SeaGreen Green ForestGreen
    syn keyword asyConstant      PineGreen LimeGreen YellowGreen SpringGreen
    syn keyword asyConstant      OliveGreen RawSienna Sepia Brown Tan Gray
    syn keyword asyConstant      Black White
  endif

  if exists("asy_syn_x11colors")
    syn keyword asyConstant      AliceBlue AntiqueWhite Aqua Aquamarine Azure
    syn keyword asyConstant      Beige Bisque Black BlanchedAlmond Blue
    syn keyword asyConstant      BlueViolet Brown BurlyWood CadetBlue
    syn keyword asyConstant      Chartreuse Chocolate Coral CornflowerBlue
    syn keyword asyConstant      Cornsilk Crimson Cyan DarkBlue DarkCyan
    syn keyword asyConstant      DarkGoldenrod DarkGray DarkGreen DarkKhaki
    syn keyword asyConstant      DarkMagenta DarkOliveGreen DarkOrange
    syn keyword asyConstant      DarkOrchid DarkRed DarkSalmon DarkSeaGreen
    syn keyword asyConstant      DarkSlateBlue DarkSlateGray DarkTurquoise
    syn keyword asyConstant      DarkViolet DeepPink DeepSkyBlue DimGray
    syn keyword asyConstant      DodgerBlue FireBrick FloralWhite ForestGreen
    syn keyword asyConstant      Fuchsia Gainsboro GhostWhite Gold Goldenrod
    syn keyword asyConstant      Gray Green GreenYellow Honeydew HotPink
    syn keyword asyConstant      IndianRed Indigo Ivory Khaki Lavender
    syn keyword asyConstant      LavenderBlush LawnGreen LemonChiffon
    syn keyword asyConstant      LightBlue LightCoral LightCyan
    syn keyword asyConstant      LightGoldenrodYellow LightGreen LightGrey
    syn keyword asyConstant      LightPink LightSalmon LightSeaGreen
    syn keyword asyConstant      LightSkyBlue LightSlateGray LightSteelBlue
    syn keyword asyConstant      LightYellow Lime LimeGreen Linen Magenta
    syn keyword asyConstant      Maroon MediumAquamarine MediumBlue
    syn keyword asyConstant      MediumOrchid MediumPurple MediumSeaGreen
    syn keyword asyConstant      MediumSlateBlue MediumSpringGreen
    syn keyword asyConstant      MediumTurquoise MediumVioletRed MidnightBlue
    syn keyword asyConstant      MintCream MistyRose Moccasin NavajoWhite
    syn keyword asyConstant      Navy OldLace Olive OliveDrab Orange
    syn keyword asyConstant      OrangeRed Orchid PaleGoldenrod PaleGreen
    syn keyword asyConstant      PaleTurquoise PaleVioletRed PapayaWhip
    syn keyword asyConstant      PeachPuff Peru Pink Plum PowderBlue Purple
    syn keyword asyConstant      Red RosyBrown RoyalBlue SaddleBrown Salmon
    syn keyword asyConstant      SandyBrown SeaGreen Seashell Sienna Silver
    syn keyword asyConstant      SkyBlue SlateBlue SlateGray Snow SpringGreen
    syn keyword asyConstant      SteelBlue Tan Teal Thistle Tomato Turquoise
    syn keyword asyConstant      Violet Wheat White WhiteSmoke Yellow
    syn keyword asyConstant      YellowGreen
  endif

  if exists("asy_syn_three")
    syn keyword asyType          path3 guide3 transform3
    syn keyword asyType          projection light material patch surface tube
    syn keyword asyConstant      currentprojection currentlight defaultrender
    syn keyword asyConstant      identity4 O X Y Z
    syn keyword asyConstant      nolight nullpens
    syn keyword asyConstant      unitsphere unithemisphere unitplane octant1
    syn keyword asyConstant      unitcone unitsolidcone unitcube unitcylinder
    syn keyword asyConstant      unitdisk unittube
  endif
endif

" string constants
syn region      asyCString       start=+'+ end=+'+ skip=+\\\\\|\\'+ contains=asyCSpecial
syn match       asyCSpecial      display contained +\\\(['"?\\abfnrtv]\|\o\{1,3}\|x[0-9A-F]\{1,2\}\|$\)+
" double quoted strings only special character is \"
syn region      asyString        start=+"+ end=+"+ skip=+\\\\\|\\"+ contains=asySpecial
syn match       asySpecial       display contained +[^\\]\(\\\\\)*\zs\\"+

" number constants
syn case ignore
syn match       asyNumbers       display transparent "\<\d\|\.\d" contains=asyNumber,asyFloat
"integer number, or floating point number without a dot and with "f".
syn match       asyNumber        display contained "\d\+"
"floating point number, with dot, optional exponent
syn match       asyFloat         display contained "\d\+\.\d*\(e[-+]\=\d\+\)\="
"floating point number, starting with a dot, optional exponent
syn match       asyFloat         display contained "\.\d\+\(e[-+]\=\d\+\)\="
"floating point number, without dot, with exponent
syn match       asyFloat         display contained "\d\+e[-+]\=\d\+"
syn case match

" comments and comment strings
if exists("asy_comment_strings")
  " A comment can contain asyString, asyCString, asyCharacter and asyNumber.
  " But a "*/" inside a asy*String in a asyComment DOES end the comment!  So we
  " need to use a special type of asy*String: asyCommentString, which also ends on
  " "*/", and sees a "*" at the start of the line as comment again.
  " Unfortunately this doesn't very well work for // type of comments :-(
  syn match     asyCommentSkip       contained "^\s*\*\($\|\s\+\)"
  syn region    asyCommentString     contained start=+L\="+ skip=+\\\\\|\\"+ end=+"+ end=+\*/+me=s-1 contains=asySpecial,asyCSpecial,asyCommentSkip
  syn region    asyCommentLString    contained start=+L\="+ skip=+\\\\\|\\"+ end=+"+ end="$" contains=asySpecial,asyCSpecial
  syn region    asyCommentL          start="//" skip="\\$" end="$" keepend contains=@asyCommentGroup,asyCommentLString,asyCharacter,asyNumbersCom,asySpaceError
  syn region    asyComment           matchgroup=asyCommentStart start="/\*" matchgroup=NONE end="\*/" contains=@asyCommentGroup,asyCommentStartError,asyCommentString,asyCharacter,asyNumbersCom,asySpaceError
else
  syn region    asyCommentL          start="//" skip="\\$" end="$" keepend contains=@asyCommentGroup,asySpaceError
  syn region    asyComment           matchgroup=asyCommentStart start="/\*" matchgroup=NONE end="\*/" contains=@asyCommentGroup,asyCommentStartError,asySpaceError
endif

" highlight common errors when starting/ending C comments
syn match       asyCommentError      display "\*/"
syn match       asyCommentStartError display "/\*"me=e-1 contained

" asyCommentGroup allows adding matches for special things in comments
syn cluster     asyCommentGroup  contains=asyTodo
syn keyword     asyTodo          contained TODO FIXME XXX

syn sync ccomment asyComment minlines=15

" delimiter matching errors
syn cluster     asyParenGroup    contains=asyParenError,asyIncluded,asySpecial,asyCSpecial,asyCommentSkip,asyCommentString,asyCommentLString,@asyCommentGroup,asyCommentStartError,asyUserCont,asyUserLabel,asyBitField,asyCommentSkip,asyOctalZero,asyCppOut,asyCppOut2,asyCppSkip,asyFormat,asyNumber,asyFloat,asyOctal,asyOctalError,asyNumbersCom
if exists("asy_no_bracket_error")
  syn region    asyParen         transparent start='(' end=')' contains=ALLBUT,@asyParenGroup,asyCppParen,asyCppString
  " asyCppParen: same as asyParen but ends at end-of-line; used in asyDefine
  syn region    asyCppParen      transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@asyParenGroup,asyParen,asyString,asyCString
  syn match     asyParenError    display ")"
  syn match     asyErrInParen    display contained "[{}]"
else
  syn region    asyParen         transparent start='(' end=')' contains=ALLBUT,@asyParenGroup,asyCppParen,asyErrInBracket,asyCppBracket,asyCppString
  " asyCppParen: same as asyParen but ends at end-of-line; used in asyDefine
  syn region    asyCppParen      transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@asyParenGroup,asyErrInBracket,asyParen,asyBracket,asyString,asyCString
if 0
  syn match     asyParenError    display "[\])]"
  syn match     asyErrInParen    display contained "[\]]"
endif
  syn region    asyBracket       transparent start='\[' end=']' contains=ALLBUT,@asyParenGroup,asyErrInParen,asyCppParen,asyCppBracket,asyCppString
  " asyCppBracket: same as asyParen but ends at end-of-line; used in asyDefine
  syn region    asyCppBracket    transparent start='\[' skip='\\$' excludenl end=']' end='$' contained contains=ALLBUT,@asyParenGroup,asyErrInParen,asyParen,asyBracket,asyString,asyCString
  syn match     asyErrInBracket  display contained "[);]"
endif

" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_asy_syn_inits")
  if version < 508
    let did_asy_syn_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif

  HiLink asyFormat               asySpecial
  HiLink asyCppString            asyString
  HiLink asyCommentL             asyComment
  HiLink asyCommentStart         asyComment
  HiLink asyLabel                Label
  HiLink asyUserLabel            Label
  HiLink asyConditional          Conditional
  HiLink asyRepeat               Repeat
  HiLink asyCharacter            Character
  HiLink asySpecialCharacter     asySpecial
  HiLink asyNumber               Number
  HiLink asyOctal                Number
  HiLink asyOctalZero            PreProc  " link this to Error if you want
  HiLink asyFloat                Float
  HiLink asyOctalError           asyError
  HiLink asyParenError           asyError
  HiLink asyErrInParen           asyError
  HiLink asyErrInBracket         asyError
  HiLink asyCommentError         asyError
  HiLink asyCommentStartError    asyError
  HiLink asySpaceError           asyError
  HiLink asySpecialError         asyError
  HiLink asyOperator             Operator
  HiLink asyStructure            Structure
  HiLink asyStorageClass         StorageClass
  HiLink asyExternal             Include
  HiLink asyPreProc              PreProc
  HiLink asyDefine               Macro
  HiLink asyIncluded             asyString
  HiLink asyError                Error
  HiLink asyStatement            Statement
  HiLink asyPreCondit            PreCondit
  HiLink asyType                 Type
  HiLink asyConstant             Constant
  HiLink asyCommentString        asyString
  HiLink asyCommentLString       asyString
  HiLink asyCommentSkip          asyComment
  HiLink asyString               String
  HiLink asyCString              String
  HiLink asyComment              Comment
  HiLink asySpecial              SpecialChar
  HiLink asyCSpecial             SpecialChar
  HiLink asyTodo                 Todo
  HiLink asyCppSkip              asyCppOut
  HiLink asyCppOut2              asyCppOut
  HiLink asyCppOut               Comment
  HiLink asyPathSpec             Statement

  delcommand HiLink
endif

let b:current_syntax = "c"
