%make movie
clear all;
screen_all = [];
warning('off','all')
load mean_Neo0
screen_all = [screen_all; mean_Neo0];
load mean_Neo1
screen_all = [screen_all; mean_Neo1];
load mean_Neo2
screen_all = [screen_all; mean_Neo2];
load mean_Neo3
screen_all = [screen_all; mean_Neo3];
load mean_Neo4
screen_all = [screen_all; mean_Neo4];
load mean_Neo5
screen_all = [screen_all; mean_Neo5];
load mean_Neo6
screen_all = [screen_all; mean_Neo6];
load mean_Neo7
screen_all = [screen_all; mean_Neo7];
load mean_Neo8
screen_all = [screen_all; mean_Neo8];
load mean_Neo9
screen_all = [screen_all; mean_Neo9];
load mean_Oracle0
screen_all = [screen_all; mean_Oracle0];
load mean_Oracle1
screen_all = [screen_all; mean_Oracle1];
load mean_Oracle2
screen_all = [screen_all; mean_Oracle2];
load mean_Oracle3
screen_all = [screen_all; mean_Oracle3];
load mean_Smith0
screen_all = [screen_all; mean_Smith0];
load mean_Smith1
screen_all = [screen_all; mean_Smith1];
load mean_Smith2
screen_all = [screen_all; mean_Smith2];
load mean_Smith3
screen_all = [screen_all; mean_Smith3];
load mean_Smith4
screen_all = [screen_all; mean_Smith4];
load mean_Smith5
screen_all = [screen_all; mean_Smith5];
load mean_Smith6
screen_all = [screen_all; mean_Smith6];
load mean_Smith7
screen_all = [screen_all; mean_Smith7];
load mean_Smith8
screen_all = [screen_all; mean_Smith8];
load mean_Smith9
screen_all = [screen_all; mean_Smith9];
for ind_v = 3:1:3
    
    if ind_v == 1
        pathv = 'C:/Projects/readMotionFile/readMotion/readMotion/20170429_121514/';
    elseif ind_v == 2
        pathv = 'C:/Projects/readMotionFile/readMotion/readMotion/20170429_132722/';
    elseif ind_v == 3
        pathv = 'C:/Projects/readMotionFile/readMotion/readMotion/20170429_141304/';
    end
    
    
    
    out_xyz = load([pathv 'out_xyz.txt']);
    start_t = out_xyz(1,1);
    end_t = out_xyz(end,1);
    t = end_t - start_t;
    Idx = find(out_xyz(:,1) == start_t);
    all_data = out_xyz(Idx(1):end,:);
    p1 = all_data(all_data(:,2) == 1, :);
    p2 = all_data(all_data(:,2) == 2, :);
    p3 = all_data(all_data(:,2) == 3, :);
    p4 = all_data(all_data(:,2) == 4, :);
    s1 = all_data(all_data(:,2) == 6, :);
    s2 = all_data(all_data(:,2) == 5, :);
    e1 = all_data(all_data(:,2) == 7, :);
    e2 = all_data(all_data(:,2) == 8, :);
    e3 = all_data(all_data(:,2) == 9, :);
    e4 = all_data(all_data(:,2) == 10, :);
    wl = all_data(all_data(:,2) == 11, :);
    wr = all_data(all_data(:,2) == 12, :);
    a1 = all_data(all_data(:,2) == 13, :);
    a2 = all_data(all_data(:,2) == 14, :);
    
    times = p1(:,1);
    
    min_x = min(all_data(:,3))-2;
    max_x = max(all_data(:,3))+2;
    min_y = min(all_data(:,5))-2;
    max_y = max(all_data(:,5))+2;
    
    framemax = size(p1,1);
    
    %colors = 'mcrgbwk';
    %markers = '+*.xsd^';
    %framemax = length(t);
    
    writerObj = VideoWriter(sprintf('%d.avi',ind_v));
    writerObj.FrameRate = framemax/(t);
    open(writerObj);
    set(gcf, 'renderer', 'zbuffer');
    set (gcf, 'Units', 'normalized', 'Position', [0,0,1,1]);
    for nf = 1:1:framemax
    %for nf = 1:1:1
        %if mod(nf,500) == 0
        %    disp(nf)
        %end
        
        whitebg([1 1 1]);
        clf;
        hold on
        [hours, minutes, seconds] = transferTime(times(nf));
        txtoftime = sprintf('Time: %d:%d:%02d',hours,minutes, floor(seconds));
        text(min_x + 0.5,max_y - 0.5,txtoftime, 'Fontsize',25);
        
        plot(p1(nf,3),p1(nf,5),'k+','MarkerSize', 20, 'DisplayName','Player 1');
        %plot(wl(nf,3),wl(nf,5),'k>','MarkerSize', 15, 'DisplayName','Player 1 wrist left');
        %plot(wr(nf,3),wr(nf,5),'k<','MarkerSize', 15, 'DisplayName','Player 1 wrist right');
        
        plot(p2(nf,3),p2(nf,5),'m*','MarkerSize', 20, 'DisplayName','Player 2');
        plot(p3(nf,3),p3(nf,5),'r.','MarkerSize', 20, 'DisplayName','Player 3');
        plot(p4(nf,3),p4(nf,5),'gx','MarkerSize', 20, 'DisplayName','Player 4');
        
        plot(s1(nf,3),s1(nf,5),'bs','MarkerSize', 20, 'DisplayName','Patient 1 (the 5th)');
        %plot(a1(nf,3),a1(nf,5),'b^','MarkerSize', 15, 'DisplayName','Patient 1 arm');
        plot(s2(nf,3),s2(nf,5),'Color',[.7 .7 0], 'Marker', 's','MarkerSize', 20, 'DisplayName','Patient 2 (the 5th)');
        %plot(a2(nf,3),a2(nf,5),'Color',[.7 .7 0], 'Marker', '^','MarkerSize', 15, 'DisplayName','Patient 2 arm');
        
        %plot(e1(nf,3),e1(nf,5),'ko','MarkerSize', 15, 'DisplayName','Equip 1');
        %plot(e2(nf,3),e2(nf,5),'mo','MarkerSize', 15, 'DisplayName','Equip 2');
        %plot(e3(nf,3),e3(nf,5),'ro','MarkerSize', 15, 'DisplayName','Equip 3');
        %plot(e4(nf,3),e4(nf,5),'go','MarkerSize', 15, 'DisplayName','Equip 4');
        
        hold on, lgd = legend('show','Location', 'west');
        hold on, lgd.FontSize = 20;
        
        plot(mean_Neo0(1),mean_Neo0(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo0');
        plot(mean_Neo1(1),mean_Neo1(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo1');
        plot(mean_Neo2(1),mean_Neo2(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo2');
        plot(mean_Neo3(1),mean_Neo3(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo3');
        plot(mean_Neo4(1),mean_Neo4(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo4');
        plot(mean_Neo5(1),mean_Neo5(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo5');
        plot(mean_Neo6(1),mean_Neo6(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo6');
        plot(mean_Neo7(1),mean_Neo7(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo7');
        plot(mean_Neo8(1),mean_Neo8(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo8');
        plot(mean_Neo9(1),mean_Neo9(3),'ks','MarkerSize', 10); %, 'DisplayName','Neo9');
        
        plot(mean_Oracle0(1),mean_Oracle0(3),'ks','MarkerSize', 10); %, 'DisplayName','Oracle0');
        plot(mean_Oracle1(1),mean_Oracle1(3),'ks','MarkerSize', 10); %, 'DisplayName','Oracle1');
        plot(mean_Oracle2(1),mean_Oracle2(3),'ks','MarkerSize', 10); %, 'DisplayName','Oracle2');
        plot(mean_Oracle3(1),mean_Oracle3(3),'ks','MarkerSize', 10); %, 'DisplayName','Oracle3');
        
        plot(mean_Smith0(1),mean_Smith0(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith0');
        plot(mean_Smith1(1),mean_Smith1(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith1');
        plot(mean_Smith2(1),mean_Smith2(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith2');
        plot(mean_Smith3(1),mean_Smith3(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith3');
        plot(mean_Smith4(1),mean_Smith4(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith4');
        plot(mean_Smith5(1),mean_Smith5(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith5');
        plot(mean_Smith6(1),mean_Smith6(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith6');
        plot(mean_Smith7(1),mean_Smith7(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith7');
        plot(mean_Smith8(1),mean_Smith8(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith8');
        plot(mean_Smith9(1),mean_Smith9(3),'ks','MarkerSize', 10); %, 'DisplayName','Smith9');
        title('Players Movement','fontsize',20)
        hold on, xlim([min_x max_x]);
        hold on, ylim([min_y max_y]);
        
        
        drawnow;
        hold off
        thisFrame = getframe(gca);
        writeVideo(writerObj, thisFrame);
        %M(nf)=thisFrame;
    end
    close(writerObj);
end
