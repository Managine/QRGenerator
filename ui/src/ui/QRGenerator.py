# -*- coding: UTF-8 -*-

from Tkinter import *
from ctypes import *
import PIL.Image
import PIL.ImageTk
import tkFont
from tkFileDialog import askopenfilename, asksaveasfilename
import tkMessageBox
import sys

sys.path.append(".")

def generate(path, string, x, y, width, minpro, adjust, sacrifice):
    dll=windll.LoadLibrary('./QRGenerator.dll')
    dll.generate.argtypes=[c_char_p, c_char_p, c_float, c_float, c_int, c_float, c_float, c_float]
    dll.generate(path,string,x,y,width,minpro,adjust,sacrifice)

class CutPicture(Frame):
    
    drawn=False
    inMoving=False
    inCutting=False
    buttonPressed=False
    inRange=False
    oldID=-1
    dx=0
    dy=0
    
    def __init__(self,master,imagePath):
        self.x=0
        self.y=0
        Frame.__init__(self, master)
        self.imageImport=PIL.Image.open(imagePath)
        self.createUI()
        self.grid()
        self.picX=self.imageImport.size[0]
        self.picY=self.imageImport.size[1]
        if self.picX>=self.picY:
            self.totalY=self.picture["height"]=self.picY*740/self.picX
            self.picture.grid(row=0,column=0,rowspan=9,columnspan=9,pady=(780-self.totalY)/2,padx=20)
            self.totalX=760
            self.width=self.picY
            self.imageDisplay=self.imageImport.resize((760,int(float(self.picY)*760/self.picX)))
        else:
            self.totalX=self.picture["width"]=self.picX*760/self.picY
            self.picture.grid(row=0,column=0,rowspan=9,columnspan=9,padx=(780-self.totalX)/2,pady=20)
            self.totalY=760
            self.width=self.picX
            self.imageDisplay=self.imageImport.resize((int(float(self.picX)*760/self.picY), 760))
            

        self.imageOb=PIL.ImageTk.PhotoImage(self.imageDisplay)
        self.picture.create_image(8,8,image=self.imageOb,anchor=NW)
        
        self.bind_all("<Button-3>", self.wmRightClick)
        self.picture.bind("<Button-1>", self.wmLeftClick)
        self.picture.bind("<Motion>", self.wmMove)
        self.picture.bind("<ButtonRelease-1>", self.wmRelease)
        
        self.bind_all("<KeyPress-Up>", self.wmOnPressArrow) #键盘操作热键
        self.bind_all("<KeyPress-Down>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-Left>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-Right>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-W>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-S>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-A>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-D>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-w>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-s>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-a>", self.wmOnPressArrow)
        self.bind_all("<KeyPress-d>", self.wmOnPressArrow)
        self.bind_all("<KeyRelease-Up>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-Down>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-Left>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-Right>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-W>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-S>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-A>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-D>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-w>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-s>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-a>", self.wmOnReleaseArrow)
        self.bind_all("<KeyRelease-d>", self.wmOnReleaseArrow)
        
        if app.hasCut:
            self.x=app.x
            self.y=app.y
            self.startX=self.x*self.totalX
            self.startY=self.y*self.totalY
            self.step=app.width*self.totalX/self.picX
            self.oldID=self.picture.create_rectangle(self.startX,self.startY, self.startX+self.step, self.startY+self.step, outline="red",width=2)
            self.drawn=True
            self.moveButton["state"]=NORMAL
    
    def wmOnPressArrow(self,event):
        if not self.drawn or not self.inMoving:
            return
        self.moveButton["state"]=DISABLED
        self.cutButton["state"]=DISABLED
        if event.keycode==38:
            self.dy=-2  
        elif event.keycode==40:
            self.dy=2      
        elif event.keycode==37:
            self.dx=-2
        elif event.keycode==39:
            self.dx=2
        elif event.keycode==87:
            self.dy=-20
        elif event.keycode==83:
            self.dy=20
        elif event.keycode==65:
            self.dx=-20
        elif event.keycode==68:
            self.dx=20
        if not 0<self.startX+self.dx<self.totalX or not 0<self.startY+self.dy<self.totalY or not 0<self.startX+self.step+self.dx<self.totalX or not 0<self.startY+self.step+self.dy<self.totalY:
            self.dx=self.dy=0
            return
        
        if not self.dx==0 or not self.dy==0:
            self.newID=self.picture.create_rectangle(self.startX+self.dx,self.startY+self.dy, self.startX+self.step+self.dx, self.startY+self.step+self.dy, outline="red",width=2)
            self.picture.delete(self.oldID)
            self.oldID=self.newID   
            self.startX=self.startX+self.dx
            self.startY=self.startY+self.dy
            self.dx=0
            self.dy=0
           
    def wmOnReleaseArrow(self,event):
        if not self.drawn or not self.inMoving:
            return
        self.moveButton["state"]=NORMAL
        self.cutButton["state"]=NORMAL
        
    def wmLeftClick(self, event):
        self.buttonPressed=True
        if self.inCutting:
            self.startX=event.x
            self.startY=event.y
        else:
            if self.inMoving:
                if self.startX<event.x<self.startX+self.step and self.startY<event.y<self.startY+self.step:
                    self.inRange=True
                    self.startMX=event.x
                    self.startMY=event.y
    
    def wmMove(self, event):
        if self.inMoving and not self.buttonPressed:       
            if self.drawn and self.startX<event.x<self.startX+self.step and self.startY<event.y<self.startY+self.step:
                self.picture["cursor"]="fleur"
            else:
                self.picture["cursor"]="hand2"
        if not self.buttonPressed:
            return
        if self.inCutting:
            if not (self.totalX>event.x>0 and self.totalY>event.y>0):
                return 
            self.curX=event.x
            self.curY=event.y
            if self.curX-self.startX>self.curY-self.startY:
                self.step=self.curY-self.startY
            else:
                self.step=self.curX-self.startX
            self.newID=self.picture.create_rectangle(self.startX, self.startY, self.startX+self.step, self.startY+self.step,outline="red",width=2)
            self.picture.delete(self.oldID)
            self.oldID=self.newID
        else:
            if self.inMoving:
                if not self.inRange:
                    return
                self.curMX=event.x
                self.curMY=event.y
                self.stepMX=self.curMX-self.startMX
                self.stepMY=self.curMY-self.startMY
                if not 0<self.startX+self.stepMX<self.totalX or not 0<self.startY+self.stepMY<self.totalY or not 0<self.startX+self.step+self.stepMX<self.totalX or not 0<self.startY+self.step+self.stepMY<self.totalY:
                    return 
                self.newID=self.picture.create_rectangle(self.startX+self.stepMX, self.startY+self.stepMY, self.startX+self.step+self.stepMX, self.startY+self.step+self.stepMY,outline="red",width=2)
                self.picture.delete(self.oldID)
                self.oldID=self.newID
    
    def wmRelease(self, event):
        self.buttonPressed=False
        if self.inCutting:
            self.drawn=True
            #self.picture["cursor"]="left_ptr"        
            #self.cutButton["state"]=NORMAL
            #self.moveButton["state"]=NORMAL
            self.onClickMoveButton()
        else:
            if self.inMoving:
                if not self.inRange:
                    return
                self.inRange=False
                self.startX=self.startX+self.stepMX
                self.startY=self.startY+self.stepMY
                #self.picture["cursor"]="left_ptr"        
                self.cutButton["state"]=NORMAL
                #self.moveButton["state"]=NORMAL
        
    def wmRightClick(self,event):
        if self.inMoving or self.inCutting:
            self.inMoving=self.inCutting=False
            self.picture["cursor"]="left_ptr"
        if not self.drawn:
            self.cutButton["state"]=NORMAL
            self.moveButton["state"]=DISABLED
        else:
            self.cutButton["state"]=NORMAL
            self.moveButton["state"]=NORMAL
            
    def onClickOK(self):
        if self.drawn:
            (x1,y1,x2,y2)=self.picture.coords(self.oldID)
            self.x=x1/float(self.imageDisplay.size[0])
            self.y=y1/float(self.imageDisplay.size[1])
            self.width=(x2-x1)/self.imageDisplay.size[0]*self.imageImport.size[0]
        else:
            self.x=0
            self.y=0
            if self.picX>=self.picY:
                self.width=self.picY
            else:
                self.width=self.picX
        app.refresh(self.x, self.y, self.width)
        app.hasCut=self.drawn
        self.master.destroy()
    
    def onClickCancel(self):
        self.master.destroy()
    
    def onClickCutButton(self):
        if self.inMoving:
            self.inMoving=False
            self.moveButton["state"]=NORMAL
        self.cutButton["state"]=DISABLED
        self.cutButton["state"]=DISABLED
        self.picture["cursor"]="cross"
        self.inCutting=True
    
    def onClickMoveButton(self):
        if self.inCutting:
            self.inCutting=False
            self.cutButton["state"]=NORMAL
        self.moveButton["state"]=DISABLED
        self.picture["cursor"]="hand2"
        self.inMoving=True       
    
    def onClickRestore(self):
        self.cutButton["state"]=NORMAL
        self.moveButton["state"]=DISABLED
        self.drawn=False
        self.inCutting=False
        self.inMoving=False
        self.picture.delete(self.oldID)
    
    def onPressKeys(self,event):
        t=event.keycode
        if t==75:
            self.onClickOK()
        elif t==65:
            self.onClickCancel()
        elif t==85:
            self.onClickCutButton()
        elif t==77:
            self.onClickMoveButton()
        elif t==82:
            self.onClickRestore()
        
    def createUI(self):
        self.bk=Canvas(self, width=1090, height=800)     #背景图片
        self.bk.grid(row=0, column=0, rowspan=9, columnspan=11, sticky=N+E+S+W)
        self.bkImage=PIL.Image.open("1.jpg")
        self.bkImage=self.bkImage.resize((1220,840))
        self.bkImageOb=PIL.ImageTk.PhotoImage(self.bkImage)
        self.bk.create_image(1090,0,anchor=NE,image=self.bkImageOb)
        
        self.picture=Canvas(self, width=740,height=740,background="#FDFDBD",relief=RIDGE,bd=8)    #裁剪主区域
        self.picture.grid(row=0,column=0,rowspan=9,columnspan=9,padx=20,pady=20)
        
        self.OK=Button(self, text="确定(K)",command=self.onClickOK,bd=4)  #按钮
        self.OK.grid(row=2, column=9,columnspan=2, padx=50,pady=5,sticky=W+E)
        self.bind_all("<Alt-KeyPress-K>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-K>", self.onPressKeys)
        
        self.Cancel=Button(self, text="取消(A)",command=self.onClickCancel,bd=4)
        self.Cancel.grid(row=3, column=9, columnspan=2, padx=50,pady=5,sticky=W+E)
        self.bind_all("<Alt-KeyPress-K>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-k>", self.onPressKeys)
        
        self.cutButton=Button(self, text="裁剪工具(U)", command=self.onClickCutButton,bd=4)
        self.cutButton.grid(row=4,column=9,padx=20,pady=40,sticky=W+E)
        self.bind_all("<Alt-KeyPress-U>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-u>", self.onPressKeys)
        
        self.moveButton=Button(self, text="移动工具(M)", command=self.onClickMoveButton, state=DISABLED,bd=4)
        self.moveButton.grid(row=4,column=10,padx=20,pady=40,sticky=W+E)
        self.bind_all("<Alt-KeyPress-M>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-m>", self.onPressKeys)
        
        self.restoreButton=Button(self, text="恢复默认(R)", command=self.onClickRestore,bd=4)
        self.restoreButton.grid(row=5,column=9, columnspan=2, padx=50, pady=5, sticky=W+E)
        self.bind_all("<Alt-KeyPress-R>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-r>", self.onPressKeys)
        
        self.font=tkFont.Font(size=14)
        self.note=Message(self,text="移动工具下可通过：\nW,S,A,D进行大量移动\n↑,↓,←,→进行少量移动",bg="#B5E61D",justify=CENTER,font=self.font,relief=GROOVE,width=300)
        self.note.grid(row=6,column=9,columnspan=2)
#         self.note=Label(self, text="请缓慢移动鼠标进行裁剪或移动！")
#         self.note.grid(row=6, column=9, columnspan=2, sticky=W+E)
        
#         self.help=Canvas(self,width=80,height=100)  #调整尺寸
#         self.help.grid(row=0, column=9,columnspan=2,rowspan=2)
#         self.help2=Canvas(self, width=80, height=360)
#         self.help2.grid(row=7, column=9, rowspan=4)
        
class Options(Frame):
    
    def __init__(self,master,m,a,s):
        Frame.__init__(self, master)
        self.createUI()
        self.grid()
        self.minpro=m
        self.adjust=a
        self.sacrifice=s
    
        self.minproScroll.set(self.minpro)
        self.sacrificeScroll.set(self.sacrifice)
        if a==1:
            self.adjustCheck.select()
        else:
            self.adjustCheck.deselect()
        
    def onClickOK(self):
        app.minpro=float(self.minpro)
        app.adjust=self.adjust
        app.sacrifice=float(self.sacrifice)
        self.master.destroy()
    
    def onClickCancel(self):
        self.master.destroy()
        
    def onValueChangedMinpro(self,value):
        self.minpro=value
        
    def onValueChangedSacrafice(self,value):
        self.sacrifice=value
    
    def onValueChangedAdjust(self):
        self.adjust=1-self.adjust
        
    def createUI(self):
        self.minProLabel=Label(self, text="最小阈值")   #标签
        self.minProLabel.grid(row=0,column=0,sticky=W+E)
        self.sacrificeLabel=Label(self, text="调整率")
        self.sacrificeLabel.grid(row=1, column=0,sticky=W+E)
        self.adjustLabel=Label(self, text="添加额外调整")
        self.adjustLabel.grid(row=2, column=0, sticky=W+E)
        
        self.OK=Button(self, text="确定", command=self.onClickOK) #按钮
        self.OK.grid(row=2, column=3, sticky=W+E, padx=10, pady=3)
        self.Cancel=Button(self, text="取消", command=self.onClickCancel)
        self.Cancel.grid(row=2, column=4, sticky=W+E, padx=10, pady=3)
    
        self.minproControl=DoubleVar()      #输入框
        self.minproEntry=Entry(self, textvariable=self.minproControl, width=10)
        self.minproEntry.grid(row=0, column=4,sticky=W+E,padx=5, pady=3)
        self.sacrificeControl=DoubleVar()
        self.sacrificeEntry=Entry(self, textvariable=self.sacrificeControl, width=10)
        self.sacrificeEntry.grid(row=1, column=4,sticky=W+E,padx=5, pady=3)
        
        self.minproScroll=Scale(self, orient=HORIZONTAL,resolution=0.05,showvalue=0,from_=0, to=1,variable=self.minproControl,command=self.onValueChangedMinpro)  #滚动条
        self.minproScroll.grid(row=0, column=1, columnspan=3, padx=10, pady=5, sticky=W+E)
        self.sacrificeScroll=Scale(self, orient=HORIZONTAL,resolution=0.05,showvalue=0,from_=0, to=1,variable=self.sacrificeControl,command=self.onValueChangedSacrafice)
        self.sacrificeScroll.grid(row=1, column=1, columnspan=3, padx=10, pady=5, sticky=W+E)
        
        self.adjustCheck=Checkbutton(self,command=self.onValueChangedAdjust)     #选项框
        self.adjustCheck.grid(row=2,column=1,sticky=N+S+W+E)
        
        self.help=Canvas(self,width=150)
        self.help.grid(row=3,column=2)
        self.help2=Canvas(self,width=80)
        self.help2.grid(row=3,column=3)
        
class MainApp(Frame):
    #fields
    pathCur=""  #选择的文件
    path=""     #用于导入图片的文件
    pathReady=""    #用于生成的文件
    savePath=""
    loadedFile=False
    importedImage=False
    generatedCode=False
    input=""
    minpro=0.15
    adjust=1
    sacrifice=0
    x=0.0
    y=0.0
    width=0
    hasCut=False
    
    def __init__(self, master=None):
        Frame.__init__(self,master)
        self.createUI()
        self.grid()
        self.bind_class("Entry", "<KeyPress-BackSpace>", self.onBackSpace) #应对tkinter的一个bug
        
    def onBackSpace(self,event):
        return
    
    def refresh(self, x, y, w):
        self.x=x
        self.y=y
        self.width=w
        self.imageToCut=self.imageImport.crop((int(self.x*self.picX), int(self.y*self.picY), int(self.x*self.picX+self.width), int(self.y*self.picY+self.width)))
        self.imageToCut=self.imageToCut.resize((400,400))
        self.imageObToCut=PIL.ImageTk.PhotoImage(self.imageToCut)
        self.picCut.delete(self.ID)
        self.ID=self.picCut.create_image(204,204,anchor=CENTER,image=self.imageObToCut)
        
    def onClickImport(self):
        if not self.loadedFile:
            tkMessageBox.showerror("错误", "请先选择图片！")
            return
                
        try:
            self.pathReady=self.path
            self.imageImport=PIL.Image.open(self.pathReady)
            self.picX=self.imageImport.size[0]
            self.picY=self.imageImport.size[1]
            if self.picX>=self.picY:
                self.width=self.picY
                #self.picInput["height"]=int(self.picY*400/self.picX)
                self.imageToCut=self.imageImport.crop((0,0,self.picY,self.picY))
                self.imageToCut=self.imageToCut.resize((400,400))
                self.imageInitial=self.imageImport.resize((400,int(float(self.picY)*400/self.picX)))
            else:
                self.width=self.picX
                #self.picInput["width"]=int(self.picX*400/self.picY)
                self.imageToCut=self.imageImport.crop((0,0,self.picX,self.picX))
                self.imageToCut=self.imageToCut.resize((400,400))
                self.imageInitial=self.imageImport.resize((int(float(self.picX)*400/self.picY), 400))
            self.imageOb=PIL.ImageTk.PhotoImage(self.imageInitial)
            self.imageObToCut=PIL.ImageTk.PhotoImage(self.imageToCut)
            #self.picInput.create_arc(10, 50, 240, 210, start=0, extent=150, fill="red")
            self.picInput.create_image(200,200,anchor=CENTER,image=self.imageOb)
            self.ID=self.picCut.create_image(204,204,anchor=CENTER,image=self.imageObToCut)
            self.x=0
            self.y=0
            self.importedImage=True
        except:
            tkMessageBox.showerror("错误","图片不存在！请检查！")
        #self.picInput.grid_remove()
        #self.picInput.grid()
    
    def onClickOption(self):
        self.hasOptionWin=True
        self.base=Toplevel()
        options=Options(self.base,self.minpro,self.adjust,self.sacrifice)
        options.master.title("选项")
        self.base.geometry("440x100+400+300")
        self.base.attributes("-alpha",0.95)
        self.base.resizable(FALSE, FALSE)
        self.base.focus_set()
        self.base.mainloop()
     
    def onClickGenerate(self):
        self.input=self.inputControl.get()
        if self.input=="":
            tkMessageBox.showerror("错误", "请输入要加密的字符串！")
            return
        if not self.importedImage:
            tkMessageBox.showerror("错误","请导入图片！")
            return
        if len(self.input)<40:
            l=40-len(self.input)
            self.input+=" "*l
        index=self.pathReady.rfind(".")
        if  not index==-1:
            if not self.pathReady[index+1:]=="bmp":
                tmp=PIL.Image.open(self.pathReady)
                tmp.save(self.pathReady[0:index+1]+"bmp")
                self.pathReady=self.pathReady[0:index+1]+"bmp"
        generate(self.pathReady, self.input, self.x,self.y,int(self.width), self.minpro, self.adjust, self.sacrifice)
        try:
            self.QRCode=PIL.Image.open("tmp.bmp")
            self.QRCode=self.QRCode.resize((385,385),PIL.Image.ANTIALIAS)
            self.QRDisplayOb=PIL.ImageTk.PhotoImage(self.QRCode)
            self.picResult.create_image(208,204,anchor=CENTER,image=self.QRDisplayOb)
            self.generatedCode=True
        except:
            tkMessageBox.showerror("错误","程序发生不明错误！\n请立即联系作者！")
    
    def onClickView(self):
        if not self.generatedCode:
            tkMessageBox.showerror("错误", "请先生成二维码！")
            return 
        self.QRCode.show()
        
    def onClickBrowse(self):
        pathCur=askopenfilename(initialdir="E:\\", defaultextension=('.bmp'),filetypes=[('bmp图像文件','.bmp'),("jpg图像文件",'.jpg'),
                                                                                        ("png图像文件",'.png'),("gif图像文件",'.gif'),
                                                                                         ("jpeg图像文件",".jpeg")])
        if pathCur:
            self.loadedFile=True
            self.pathControl.set(pathCur)
            self.path=pathCur
    
    def onClickCut(self):
        if not self.importedImage:
            tkMessageBox.showerror("错误", "请先导入图片！")
            return 
        self.base2=Toplevel()
        cutPicture=CutPicture(self.base2,self.pathReady)
        cutPicture.master.title("裁剪")
        self.base2.geometry("1090x800+300+10")
        self.base2.attributes("-alpha",0.95)
        self.base2.resizable(FALSE, FALSE)
        self.base2.focus_set()
        self.base2.mainloop()
        return    
    
    def onClickSave(self):
        if not self.generatedCode:
            tkMessageBox.showinfo("提示", "请先生成二维码")
        savePath=asksaveasfilename(initialdir="E:\\",defaultextension='.bmp',initialfile="output.bmp")
        try:
            open(savePath, "wb").write(open("tmp.bmp", "rb").read())
        except:
            pass
        return    
    
    def onClickQuit(self):
        self.master.destroy()
    
    def onPressKeys(self,event):
        t=event.keycode
        if t==73:
            self.onClickImport()
        elif t==66:
            self.onClickBrowse()
        elif t==79:
            self.onClickOption()
        elif t==71:
            self.onClickGenerate()
        elif t==67:
            self.onClickCut()
        elif t==86:
            self.onClickView()
        elif t==83:
            self.onClickSave()
        elif t==81:
            self.onClickQuit()
        
    def createUI(self):
        self.bk=Canvas(self, width=1350, height=700)        #背景图片
        self.bk.grid(row=0, column=0, columnspan=9, rowspan=9, sticky=N+W+E+S)
        self.bkImage=PIL.Image.open("0.jpg")
        self.bkImage=self.bkImage.resize((1380,750))
        self.bkImageOb=PIL.ImageTk.PhotoImage(self.bkImage)
        self.bk.create_image(0,0,anchor=NW,image=self.bkImageOb)
        
        self.labelInput=Label(self,text="加密字符串：",background="#D0D2DF")    #字符串标签
        self.labelPath=Label(self, text="选择的图片地址：",background="#C8CEDA")
        self.labelInput.grid(row=0,column=0,sticky=W+E,padx=20, pady=3)
        self.labelPath.grid(row=1, column=0,sticky=W+E,padx=20, pady=3)
        
        self.inputControl=StringVar()   #输入信息模块
        self.inputString=Entry(self,textvariable=self.inputControl)
        self.inputString.grid(row=0,column=1, columnspan=7, sticky=W+E)
        
        self.pathControl=StringVar()    #图片选择模块
        self.pathEntry=Entry(self, textvariable=self.pathControl,state="disabled")
        self.pathEntry.grid(row=1,column=1,columnspan=6,sticky=W+E)
        
        self.impButton=Button(self, text="导入图片(I)", command=self.onClickImport, bd=4)    #按钮群
        self.impButton.grid(row=0, column=8,sticky=N+S+E+W,padx=8, pady=5)
        self.bind_all("<Alt-KeyPress-I>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-i>", self.onPressKeys)
        
        self.optButton=Button(self, text="选项(O)", command=self.onClickOption,bd=4) 
        self.optButton.grid(row=1, column=8,sticky=N+S+E+W,padx=8, pady=5)
        self.bind_all("<Alt-KeyPress-O>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-o>", self.onPressKeys)
        
        self.impButton=Button(self, text="生成二维码(G)", command=self.onClickGenerate,bd=4) 
        self.impButton.grid(row=2, column=8,padx=8, pady=5,sticky=N+S+E+W)
        self.bind_all("<Alt-KeyPress-G>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-g>", self.onPressKeys)
        
        self.impButton=Button(self, text="浏览(B)", command=self.onClickBrowse,bd=4) 
        self.impButton.grid(row=1, column=7, sticky=W+E,padx=30, pady=5)
        self.bind_all("<Alt-KeyPress-B>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-b>", self.onPressKeys)
        
#         self.note=Label(self, text="注意：仅支持bmp格式图片的导入和处理",background="#DEE2ED")       #图片显示区
#         self.note.grid(row=2, column=1, columnspan=7, padx=350, pady=3, stick=W+E)
        
        self.picInput=Canvas(self,width=400, height=400,background="#EAFFF8",relief=RIDGE,borderwidth=8)
        self.picInput.grid(row=3, column=0, columnspan=3, rowspan=3, padx=15, pady=5)        
        self.picCut=Canvas(self,width=400, height=400,background="#EAFFF8",relief=RIDGE,borderwidth=8)
        self.picCut.grid(row=3, column=3, columnspan=3, rowspan=3, padx=15, pady=5)        
        self.picResult=Canvas(self,width=400, height=400,background="#EAFFF8",relief=RIDGE,borderwidth=8)
        self.picResult.grid(row=3, column=6, columnspan=3, rowspan=3, padx=15,pady=5)
        
        self.labelPic1=Label(self,text="导入的图片",background="#C2DBEF")
        self.labelPic2=Label(self, text="参与生成部分",background="#C2DBEF")
        self.labelPic3=Label(self, text="生成结果",background="#C2DBEF")
        self.labelPic1.grid(row=6,column=1,sticky=W+E,padx=30, pady=3)
        self.labelPic2.grid(row=6,column=4,sticky=W+E,padx=30, pady=3)
        self.labelPic3.grid(row=6, column=7,sticky=W+E,padx=30, pady=3)
        
        self.cut=Button(self,text="裁剪图片(C)", command=self.onClickCut,background="#4B90BA",bd=4,activebackground="#4B90BA")
        self.cut.grid(row=7,column=1,padx=30, pady=5,sticky=W+E)     
        self.bind_all("<Alt-KeyPress-C>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-c>", self.onPressKeys)
        
        self.save=Button(self,text="查看大图(V)", command=self.onClickView,background="#C2DBEF",bd=4,activebackground="#C2DBEF")
        self.save.grid(row=7,column=7,padx=30, pady=5,sticky=W+E)  
        self.bind_all("<Alt-KeyPress-V>", self.onClickView)
        self.bind_all("<Alt-KeyPress-v>", self.onClickView)
        
        self.save=Button(self,text="保存图片(S)", command=self.onClickSave,background="#C2DBEF",bd=4,activebackground="#C2DBEF")
        self.save.grid(row=7,column=8,padx=30, pady=5,sticky=W+E)
        self.bind_all("<Alt-KeyPress-S>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-s>", self.onPressKeys)
        
        self.quitButton=Button(self, text="退出程序(Q)", command=self.onClickQuit,bd=4)
        self.quitButton.grid(row=8, column=8,padx=15,pady=10,sticky=W+E)
        self.bind_all("<Alt-KeyPress-Q>", self.onPressKeys)
        self.bind_all("<Alt-KeyPress-q>", self.onPressKeys)
        
root=Tk()
app=MainApp(root)
app.master.title("二维码生成器")
root.geometry("1350x700+100+60")
root.attributes("-alpha",0.98)
root.resizable(FALSE, FALSE)
app.mainloop() 
