function varargout = FILT_GUI(varargin)
% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @FILT_GUI_OpeningFcn, ...
                   'gui_OutputFcn',  @FILT_GUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT
return
% --- Executes just before FILT_GUI is made visible.
function FILT_GUI_OpeningFcn(hObject, eventdata, handles, varargin) %#ok<INUSL>
% Choose default command line output for FILT_GUI
handles.output = hObject;
clc;
% Update handles structure
guidata(hObject, handles);
return
% --- Outputs from this function are returned to the command line.
function varargout = FILT_GUI_OutputFcn(hObject, eventdata, handles)  %#ok<INUSL>


% Get default command line output from handles structure
varargout{1} = handles.output;
return
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function PFILT(handles)
Fs = 44100;
clc;

b = [ get(handles.b0, 'Value') get(handles.b1, 'Value')...
      get(handles.b2, 'Value') get(handles.b3, 'Value')];
b = 3 * b

a = [ get(handles.a0, 'Value') get(handles.a1, 'Value')...
      get(handles.a2, 'Value') get(handles.a3, 'Value')];
if(min(a)==0 && max(a)==0)
    a(end) = 1
else
    a = 3 * a
end


[ H, F ] = freqz( b, a, 512,  1 );
a1 = subplot( 1, 1, 1, 'Parent', handles.PLOTS );
F = Fs * F;
H = abs(H);
plot( a1, F, H );
axis( a1, [0 Fs/4 0 (max(H)+1)] );

return
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --- Executes on slider movement.
function b0_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
PFILT(handles);
return
% --- Executes on slider movement.
function b1_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
PFILT(handles);
return
% --- Executes on slider movement.
function b2_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
PFILT(handles);
return
% --- Executes on slider movement.
function b3_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
PFILT(handles);
return
% --- Executes on slider movement.
function a0_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
PFILT(handles);
return
% --- Executes on slider movement.
function a1_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
PFILT(handles);
return
% --- Executes on slider movement.
function a2_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
PFILT(handles);
return
% --- Executes on slider movement.
function a3_Callback(hObject, eventdata, handles) %#ok<INUSL,DEFNU>
% hObject    handle to a3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of
%        slider
PFILT(handles);
return
