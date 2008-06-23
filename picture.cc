/*****
 * picture.cc
 * Andy Hammerlindl 2002/06/06
 *
 * Stores a picture as a list of drawElements and handles its output to 
 * PostScript. 
 *****/

#include <csignal>

#include "errormsg.h"
#include "picture.h"
#include "util.h"
#include "settings.h"
#include "interact.h"

using std::ifstream;
using std::ofstream;

using namespace settings;

texstream::~texstream() {
  if(!getSetting<bool>("keep")) {
    unlink("texput.log");
    unlink("texput.out");
    unlink("texput.aux");
    if(settings::pdf(texengine()))
      unlink("texput.pdf");
  }
}

namespace camp {

const char *texpathmessage() {
  ostringstream buf;
  buf << "the directory containing your " << getSetting<string>("tex")
      << " engine (" << texengine() << ")";
  return Strdup(buf.str());
}
  
picture::~picture()
{
}

bool picture::epsformat,picture::pdfformat,picture::xobject, picture::pdf;
bool picture::Labels;
double picture::paperWidth,picture::paperHeight;
  
void picture::enclose(drawElement *begin, drawElement *end)
{
  assert(begin);
  assert(end);
  nodes.push_front(begin);
  lastnumber=0;
  for(nodelist::iterator p=nodes.begin(); p != nodes.end(); ++p) {
    assert(*p);
    if((*p)->islayer()) {
      nodes.insert(p,end);
      ++p;
     while(p != nodes.end() && (*p)->islayer()) ++p;
     if(p == nodes.end()) return;
     nodes.insert(p,begin);
    }
  }
  nodes.push_back(end);
}

// Insert at beginning of picture.
void picture::prepend(drawElement *p)
{
  assert(p);
  nodes.push_front(p);
  lastnumber=0;
}

void picture::append(drawElement *p)
{
  assert(p);
  nodes.push_back(p);
}

void picture::add(picture &pic)
{
  if (&pic == this) return;

  // STL's funny way of copying one list into another.
  copy(pic.nodes.begin(), pic.nodes.end(), back_inserter(nodes));
}

// Insert picture pic at beginning of picture.
void picture::prepend(picture &pic)
{
  if (&pic == this) return;
  
  copy(pic.nodes.begin(), pic.nodes.end(), inserter(nodes, nodes.begin()));
  lastnumber=0;
}

bool picture::havelabels()
{
  size_t n=nodes.size();
  if(n > lastnumber && !labels && getSetting<string>("tex") != "none") {
    // Check to see if there are any labels yet
    nodelist::iterator p=nodes.begin();
    for(size_t i=0; i < lastnumber; ++i) ++p;
    for(; p != nodes.end(); ++p) {
      assert(*p);
      if((*p)->islabel())
        labels=true;
    }
  }
  return labels;
}

bbox picture::bounds()
{
  size_t n=nodes.size();
  if(n == lastnumber) return b_cached;
  
  if(lastnumber == 0) { // Maybe these should be put into a structure.
    b_cached=bbox();
    labelbounds.clear();
    bboxstack.clear();
  }
  
  if(havelabels()) texinit();
  
  nodelist::iterator p=nodes.begin();
  for(size_t i=0; i < lastnumber; ++i) ++p;
  for(; p != nodes.end(); ++p) {
    assert(*p);
    (*p)->bounds(b_cached,processData().tex,labelbounds,bboxstack);
    
     // Optimization for interpreters with fixed stack limits.
    if((*p)->endclip()) {
      nodelist::iterator q=p;
      if(q != nodes.begin()) {
	--q;
	assert(*q);
	if((*q)->endclip())
	  (*q)->save(false);
      }
    }
  }

  lastnumber=n;
  return b_cached;
}

void picture::texinit()
{
  drawElement::lastpen=pen(initialpen);
  processDataStruct &pd=processData();
  // Output any new texpreamble commands
  if(pd.tex.isopen()) {
    if(pd.TeXpipepreamble.empty()) return;
    texpreamble(pd.tex,pd.TeXpipepreamble,false);
    pd.TeXpipepreamble.clear();
    return;
  }
  
  ostringstream cmd;
  cmd << "'" << texprogram() << "'" << " \\scrollmode";
  pd.tex.open(cmd.str().c_str(),"texpath",texpathmessage());
  pd.tex.wait("\n*");
  pd.tex << "\n";
  texdocumentclass(pd.tex,true);
  
  texdefines(pd.tex,pd.TeXpreamble,true);
  pd.TeXpipepreamble.clear();
}
  
bool picture::texprocess(const string& texname, const string& outname,
			 const string& prefix, const pair& bboxshift) 
{
  int status=0;
  ifstream outfile;
  
  outfile.open(texname.c_str());
  if(outfile) {
    outfile.close();
    ostringstream cmd;
    cmd << "'" << texprogram() << "'"
	<< " \\nonstopmode\\input '" << texname << "'";
    bool quiet=verbose <= 1;
    status=System(cmd,quiet ? 1 : 0,"texpath",texpathmessage());
    if(!status && getSetting<bool>("twice"))
      status=System(cmd,quiet ? 1 : 0,"texpath",texpathmessage());
    if(status) {
      if(quiet) {
	ostringstream cmd;
	cmd << "'" << texprogram() << "'"
	    << " \\scrollmode\\input '" << texname << "'";
	System(cmd,0);
      }
      return false;
    }
    
    if(!pdf) {
      string dviname=auxname(prefix,"dvi");
      string psname=auxname(prefix,"ps");
    
      double height=b.top-b.bottom+1.0;
    
      // Magic dvips offsets:
      double hoffset=-128.4;
      double vertical=height;
      if(!latex(getSetting<string>("tex"))) vertical += 2.0;
      double voffset=(vertical < 13.0) ? -137.8+vertical : -124.8;

      hoffset += b.left+bboxshift.getx();
      voffset += paperHeight-height-b.bottom-bboxshift.gety();
    
      ostringstream dcmd;
      dcmd << "'" << getSetting<string>("dvips") << "' -R -Pdownload35 -D600"
	   << " -O " << hoffset << "bp," << voffset << "bp"
	   << " -T " << paperWidth << "bp," << paperHeight << "bp "
           << getSetting<string>("dvipsOptions");
      if(verbose <= 1) dcmd << " -q";
      dcmd << " -o '" << psname << "' '" << dviname << "'";
      status=System(dcmd,0,true,"dvips");
      if(status != 0) return false;
    
      ifstream fin(psname.c_str());
      psfile fout(outname,false);
    
      string s;
      bool first=true;
      transform t=shift(bboxshift)*T;
      bool shift=!t.isIdentity();
      string beginspecial="TeXDict begin @defspecial";
      string endspecial="@fedspecial end";
      while(getline(fin,s)) {
	if(s.find("%%DocumentPaperSizes:") == 0) continue;
	if(s.find("%!PS-Adobe-") == 0) {
	  fout.header();
	} else if(first && s.find("%%BoundingBox:") == 0) {
	  bbox box=b;
	  box.shift(bboxshift);
	  if(verbose > 2) BoundingBox(cout,box);
	  fout.BoundingBox(box);
	  first=false;
	} else if(shift && s.find(beginspecial) == 0) {
	  fout.verbatimline(s);
	  fout.gsave();
	  fout.concat(t);
	} else if(shift && s.find(endspecial) == 0) {
	  fout.grestore();
	  fout.verbatimline(s);
	} else
	  fout.verbatimline(s);
      }
      if(!getSetting<bool>("keep")) { // Delete temporary files.
	unlink(dviname.c_str());
	unlink(psname.c_str());
      }
    }
      
    if(!getSetting<bool>("keep")) { // Delete temporary files.
      unlink(texname.c_str());
      if(!getSetting<bool>("keepaux")) unlink(auxname(prefix,"aux").c_str());
      unlink(auxname(prefix,"log").c_str());
      unlink(auxname(prefix,"out").c_str());
    }
    if(status == 0) return true;
  }
  return false;
}

int picture::epstopdf(const string& epsname, const string& pdfname)
{
  ostringstream cmd;
  
  cmd << "'" << getSetting<string>("gs")
      << "' -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -dEPSCrop"
      << " -dSubsetFonts=true -dEmbedAllFonts=true -dMaxSubsetPct=100"
      << " -dPDFSETTINGS=/prepress -dCompatibilityLevel=1.4";
  if(safe)
    cmd << " -dSAFER";
  if(!getSetting<bool>("autorotate"))
    cmd << " -dAutoRotatePages=/None";
  cmd << " -g" << max(ceil(paperWidth),1.0) << "x" << max(ceil(paperHeight),1.0)
      << " -dDEVICEWIDTHPOINTS=" << max(b.right-b.left,3.0)
      << " -dDEVICEHEIGHTPOINTS=" << max(b.top-b.bottom,3.0)
      << " " << getSetting<string>("gsOptions")
      << " -sOutputFile='" << pdfname << "' '" << epsname << "'";
  return System(cmd,0,true,"gs","Ghostscript");
}
  
static mem::map<CONST string,int> pids;

bool picture::postprocess(const string& prename, const string& outname,
			  const string& outputformat, double magnification,
			  bool wait, bool view)
{
  int status=0;
  
  if((pdf && Labels) || !epsformat) {
    if(pdfformat) {
      if(pdf && Labels) status=rename(prename.c_str(),outname.c_str());
      else status=epstopdf(prename,outname);
    } else {
      ostringstream cmd;
      double expand=2.0;
      double res=expand*72.0;
      cmd << "'" << getSetting<string>("convert") 
	  << "' -density " << res << "x" << res
	  << " +antialias -geometry " << 100.0/expand << "%x"
	  << " '" << nativeformat()+":" << prename << "'"
          << " '" << outputformat << ":" << outname << "'";
      status=System(cmd,0,true,"convert");
    }
    if(!getSetting<bool>("keep")) unlink(prename.c_str());
  }
  if(status != 0) return false;
  
  if(verbose > 0)
    cout << "Wrote " << outname << endl;
  if(settings::view() && view) {
    if(epsformat || pdfformat) {
      // Check to see if there is an existing viewer for this outname.
      mem::map<CONST string,int>::iterator p=pids.find(outname);
      bool running=(p != pids.end());
      string Viewer=pdfformat ? getSetting<string>("pdfviewer") :
	getSetting<string>("psviewer");
      int pid;
      if(running) {
	pid=p->second;
	if(interact::interactive && pid)
	  running=(waitpid(pid, &status, WNOHANG) != pid);
      }
	
      if(running) {
	if(Viewer == "gv") kill(pid,SIGHUP); // Tell gv to reread file.
	else if(pdfformat && !b.empty) {
	  // Kill pdfviewer so that file can be redrawn.
	  kill(pid,SIGINT);
	  while(waitpid(pid, &status, 0) != pid);
	  running=false;
	}
      }
      if(!running) {
	ostringstream cmd;
	cmd << "'" << Viewer << "'";
	cmd << " '" << outname << "'";
	status=System(cmd,0,wait,
		      pdfformat ? "pdfviewer" : "psviewer",
		      pdfformat ? "your PDF viewer" : "your PostScript viewer",
		      &pid);
	pids[outname]=pid;
	if(status != 0) return false;
      }
    } else {
      ostringstream cmd;
      cmd << "'" << getSetting<string>("display") << "' '"
	  << outname << "'";
      string application="your "+outputformat+" viewer";
      status=System(cmd,0,wait,"display",application.c_str());
      if(status != 0) return false;
    }
  }
  
  return true;
}

string Outname(const string& prefix, const string& outputformat,
	       bool standardout)
{
  return (standardout ? "-" : buildname(prefix,outputformat,"",
					prefix != settings::outname() &&
					!globalwrite()));
}

bool picture::shipout(picture *preamble, const string& Prefix,
		      const string& format, double magnification,
		      bool wait, bool view)
{
  b=bounds();
  
  bool TeXmode=getSetting<bool>("inlinetex") && 
    getSetting<string>("tex") != "none";
  Labels=labels || TeXmode;
  
  pdf=settings::pdf(getSetting<string>("tex"));
  
  bool standardout=Prefix == "-";
  string prefix=standardout ? "out" : Prefix;
  string preformat=nativeformat();
  string outputformat=format.empty() ? defaultformat() : format;
  epsformat=outputformat == "eps";
  pdfformat=outputformat == "pdf";
  xobject=magnification > 0;
  string outname=Outname(prefix,outputformat,standardout);
  string epsname=epsformat ? (standardout ? "" : outname) :
    auxname(prefix,"eps");
  string prename=((epsformat && !pdf) || !Labels) ? epsname : 
    auxname(prefix,preformat);
  
  if((b.empty && !Labels)) { // Output a null file
    bbox b;
    b.left=b.bottom=0;
    b.right=b.top=xobject ? 18 : 1;
    psfile out(epsname,false);
    out.prologue(b);
    out.epilogue();
    out.close();
    return postprocess(epsname,outname,outputformat,1.0,wait,view);
  }
  
  bool pngxformat=xobject && getSetting<string>("xformat") == "png";
  if(pngxformat) {
    // Work around half-pixel bounding box bug in Ghostscript pngalpha driver
    double fuzz=0.5/magnification;
    b.top += fuzz;
    b.right += fuzz;
    b.bottom -= fuzz;
  }
    
  SetPageDimensions();
  
  paperWidth=getSetting<double>("paperwidth");
  paperHeight=getSetting<double>("paperheight");
  Int origin=getSetting<Int>("align");
    
  pair bboxshift=(origin == ZERO && !pdfformat) ?
    pair(0.0,0.0) : pair(-b.left,-b.bottom);
  if(!pdfformat) {
    bboxshift += getSetting<pair>("offset");
    if(origin != ZERO && origin != BOTTOM) {
      double yexcess=max(paperHeight-(b.top-b.bottom+1.0),0.0);
      if(origin == TOP) bboxshift += pair(0.0,yexcess);
      else {
	double xexcess=max(paperWidth-(b.right-b.left+1.0),0.0);
	bboxshift += pair(0.5*xexcess,0.5*yexcess);
      }
    }
  }
  
  bool status = true;
  
  string texname;
  texfile *tex=NULL;
  
  if(Labels) {
    spaceToUnderscore(prefix);
    texname=auxname(prefix,"tex");
    tex=new texfile(texname,b);
    tex->prologue();
  }
  
  nodelist::iterator layerp=nodes.begin();
  nodelist::iterator p=layerp;
  unsigned layer=0;
  mem::list<string> psnameStack;
  
  bbox bshift=b;
  
  transparency=false;
  
  while(p != nodes.end()) {
    string psname,pdfname;
    if(Labels) {
      ostringstream buf;
      buf << prefix << "_" << layer;
      psname=buildname(buf.str(),"eps");
      if(pdf) pdfname=buildname(buf.str(),"pdf");
    } else {
      psname=epsname;
      bshift.shift(bboxshift);
    }
    psnameStack.push_back(psname);
    if(pdf) psnameStack.push_back(pdfname);
    psfile out(psname,pdfformat);
    out.prologue(bshift);
  
    if(Labels) tex->beginlayer(pdf ? pdfname : psname);
    else {
      out.gsave();
      out.translate(bboxshift);
    }
  
    if(preamble) {
      // Postscript preamble.
      nodelist Nodes=preamble->nodes;
      nodelist::iterator P=Nodes.begin();
      if(P != Nodes.end()) {
	out.resetpen();
	for(; P != Nodes.end(); ++P) {
	  assert(*P);
	  if(!(*P)->draw(&out))
	    status = false;
	}
      }
    }
    out.resetpen();
    
    for(; p != nodes.end(); ++p) {
      assert(*p);
      if(Labels && (*p)->islayer()) break;
      if(!(*p)->draw(&out))
	status = false;
    }
    if(!Labels) out.grestore();
    
    out.epilogue();
    out.close();
    
    if(out.Transparency())
      transparency=true;
    
    if(Labels) {
      tex->resetpen();
      if(status) {
	if(pdf && !b.empty) {
	  status=(epstopdf(psname,pdfname) == 0);
	  if(!getSetting<bool>("keep")) unlink(psname.c_str());
	}
	
	if(status) {
	  for (p=layerp; p != nodes.end(); ++p) {
	    if((*p)->islayer()) {
	      tex->endlayer();
	      layerp=++p;
	      layer++;
	      break;
	    }
	    assert(*p);
	    if(!(*p)->write(tex,b))
	      status = false;
	  }
	}
      }    
    }
  }
  
  if(status) {
    if(TeXmode) {
      if(Labels && verbose > 0) cout << "Wrote " << texname << endl;
      delete tex;
    } else {
      if(Labels) {
	tex->epilogue();
	status=texprocess(texname,prename,prefix,bboxshift);
	delete tex;
	if(!getSetting<bool>("keep")) {
	  for(mem::list<string>::iterator p=psnameStack.begin();
	      p != psnameStack.end(); ++p)
	    unlink(p->c_str());
	}
      }
      if(status) {
	if(xobject) {
	  if(transparency && pngxformat)
	    status=(epstopdf(prename,Outname(prefix,"pdf",standardout)) == 0);
	} else
	  status=postprocess(prename,outname,outputformat,magnification,wait,
			     view);
      }
    }
  }
  
  if(!status) reportError("shipout failed");
    
  return true;
}

bool picture::shipout3(const string& prefix)
{
  bool status = true;
  string prcname=buildname(prefix,"prc");
  prcfile prc(prcname);
  
  for(nodelist::iterator p=nodes.begin(); p != nodes.end(); ++p) {
    assert(*p);
    if(!(*p)->write(&prc))
      status = false;
  }
  
  if(status)
    status=prc.finish();
  
  if(!status) reportError("shipout3 failed");
    
  if(verbose > 0) cout << "Wrote " << prcname << endl;
  
  return true;
}

picture *picture::transformed(const transform& t)
{
  picture *pic = new picture;

  nodelist::iterator p;
  for (p = nodes.begin(); p != nodes.end(); ++p) {
    assert(*p);
    pic->append((*p)->transformed(t));
  }
  pic->T=transform(t*T);

  return pic;
}


} // namespace camp
