#! /usr/bin/env ruby

=begin
This ruby script takes an XBM image and reformats it to that of our OLED
display.

Our OLED contains rows of 8-bit high columns. Each column in one byte. Each row
is one byte high.  For example, a 128x32 pixel display contains 4 rows of 128
bytes each.

XBM image formats are already in C code, making them easy to process as text. I
use ToyViewer on my Mac to convert PNG or whatever other format to monochrome
XBM files.

XBM format is different. Each row is one pixel high, with one-byte columns.
Each byte is also reversed, such that the very first or top, left pixel in a
bitmap is the lowest bit in the first byte. The 9th pixel (left to right) is
the lowest bit in the second byte and so on.  Oh and, if the number of pixels
on a line is not a muliple of 8, then the extra required to meet that condition
are simply added as junk bits in the last byte on each row.
=end

def main(xbm, debug)

  base=File.basename(xbm, ".xbm")
  path=File.dirname(xbm)
  cols,rows=1,1
  if (base.sub! /_(\d+)x(\d+)/,"")
    cols,rows = $1.to_i,$2.to_i
  end
  
  File.read(xbm)=~/_width\s+(\d+).*_height\s+(\d+).*\{(.*)\}/m
  w,h,bytes=$1.to_i,$2.to_i,$3
  bytes=eval "["+bytes+"]"

  if debug == 1
    # just for kicks and to confirm that we're reading the image correctly, let's print it as ASCII
    puts
    printf("Characters are %dx%d pixels, according to font bitmap's filename.\r\n", cols, rows)
    printf("Input XBM Image is %dx%d pixels ",w,h)
    printf("and they goes a little something like this ...\r\n")
    puts
    h.times{|y|
      w.times{|x|
        if (bytes[(x + (y * w))/8] & (1 << (x % 8))) > 0
          print "*" 
        else
          print " "
        end
      }
      puts
    }
    puts
  end

  # now for the tricky part

  wbytes = (w + 7) / 8  # rounds up to nearest multiple of 8 that will contain w pixels 
  hbytes = (h + 7) / 8  # ditto for height
  
  if debug == 1
    printf("XBM bytes per line  = %d\r\n", wbytes)
    printf("XBM height in bytes = %d\r\n", hbytes)
  end

  out = Array.new(w * h, 0x00)
  hbytes.times{|oled_row|
    wbytes.times{|xbm_col|
      # build an OLED column, top (LSB) to bottom
      0.upto(7){|xb|  # run along the 8 bits of of this xbm column
        oled_col = xbm_col * 8 + xb
        oled_bits = 0x00
        0.upto(7){|b| # checking 8 bits deep, as we go
          xbm_row = (oled_row * 8) + b
          xbm_idx = xbm_col + (xbm_row * wbytes)
          if xbm_idx < bytes.count
            xbm_byte = bytes[xbm_idx]
            oled_bits |= (1 << b) if ( xbm_byte & (1<<xb) ) > 0 
          end
        }
        out[oled_col + ((wbytes * 8) * oled_row)] = oled_bits
      }
    }
  }
  if debug == 1
    # Now let's output that as ASCII, so we can see if it worked!
    printf("... remaps to ...\r\n\n");
    puts
    hbytes.times{|oled_row|
      0.upto(7){|b| # 7 bits per oled row (7 rows on our screen)
        (wbytes * 8).times{|oled_col|
          col_byte = out[oled_col + (oled_row * (wbytes * 8))]
          if (col_byte & (1<<b)) > 0
            print "O"
          else
            print " "
          end
        }
        puts
      }
    }
    puts
  end

  # now create a .c file with out new data
  File.open("#{path}/#{base}_#{cols}x#{rows}.c", "w"){|f|
    f.printf("#include \"fonts.h\"\n\n")
    f.printf("Font %s_%dx%d = {\n",base.upcase,cols,rows)
    f.printf("  %d, // width of one character, in pixels\n",cols)
    f.printf("  %d, // height of one character, in pixels\n",rows)
    f.printf("  %d, // width of bitmap, in pixels\n",(wbytes * 8))
    f.printf("  %d, // height of bitnap, in pixels\n",(hbytes * 8))
    f.printf("  {\n",base.upcase,cols,rows,rows)
    count = 0
    hbytes.times{|row|
      (wbytes * 8).times{|col|
        f.printf("    ") if count == 0
        f.print(",") if count > 0
        f.print "\n    " if count > 0 && count % 16 == 0
        f.printf("0x%02x", out[col + (row * wbytes * 8)])
        count += 1
      }
    }
    f.printf("\n  }\n")
    f.printf("};\n")
  }
end

def help()
  puts "\nDot XBM to dot C Image File Converter"
  puts "Converts .xbm image file to a C language includable .c file, 'casue like, why not?"
  puts "Usage:"
  puts "      #{ File.basename(__FILE__) } <filename.xbm>\n"
  puts "      Options:"
  puts "         --help  (-h)   # this help message"
  puts "         --debug (-d)   # pruduce verbose debug output"
  abort
end

if ARGV.empty?
  help()
else
  aliases = {
    "-h" => "--help",
    "-d" => "--debug"
  }
  xbm = ARGV.shift
  opt = ARGV.shift
  opt = aliases[opt] || opt

  debug = 0
  case opt 
  when "--debug"
    debug = 1
  when "--help"
    help()
    exit
  end

  main(xbm, debug)
end
