clear all;
path = 's2a';
%start_m = 54;
%end_m = 66;
%time = minutes(start_m):seconds(1):minutes(end_m);

%look each
if (0)   
    fid = fopen([path '/out_lookEachOther.txt']);
    tline = fgetl(fid);
    if (ischar(tline))
        numbers = str2num(tline);
    else
        break;
    end
    [h, m, s] = transferTime(numbers(1));
    inds = (m - start_m) * 60 + ceil(s);
    for i = 2:3:length(numbers)
        prob = numbers(i);
        id1 = numbers(i + 1);
        id2 = numbers(i + 2);
        if id1 > id2
            temp = id1;
            id1 = id2;
            id2 = temp;
        end
        var_cur = sprintf('look_%d%d',id1, id2);
        if (~exist(var_cur))
            eval([var_cur '= [inds prob];']);
        else
            eval([var_cur '=[' var_cur ';[inds prob]];']);
        end
    end
    
    while ischar(tline)
        tline = fgetl(fid);
        if (ischar(tline))
            numbers = str2num(tline);
        else
            break;
        end
        [h, m, s] = transferTime(numbers(1));
        inds = (m - start_m) * 60 + ceil(s);
        for i = 2:3:length(numbers)
            prob = numbers(i);
            id1 = numbers(i + 1);
            id2 = numbers(i + 2);
            if id1 > id2
                temp = id1;
                id1 = id2;
                id2 = temp;
            end
            var_cur = sprintf('look_%d%d',id1, id2);
            if (~exist(var_cur))
                eval([var_cur '= [inds prob];']);
            else
                eval([var_cur '=[' var_cur ';[inds prob]];']);
            end
        end
    end
    
    fclose(fid);



    looknames = who('look_*');
    colors = 'ymcrgbwk';
    markers = 'o+*.xsd^';
    all_all_prob = [];
    figure,
    for i = 1:1:length(looknames)
        cur_look = looknames{i};
        eval(['all_inds = ' cur_look '(:,1);']);
        eval(['all_probs = ' cur_look '(:,2);']);
        all_all_prob = [all_all_prob;all_probs];
        cur_color = colors(i);
        cur_marker = markers(i);
        sign = [cur_color cur_marker];
        legend_txt = ['Between ' cur_look(6) ' and ' cur_look(7)];
        hold on, plot(time(all_inds), all_probs, sign, 'MarkerSize', 15, 'DisplayName',legend_txt);
    end
    
    hold on, xlim([start_m end_m]);
    hold on, ylim([min(all_all_prob)-0.01 max(all_all_prob)+0.01]);
    %hold on, ax = gca;
    %date1 = datenum(num2str(start_m),'MM');
    %date2 = datenum(num2str(end_m),'MM');
    %xData = linspace(date1,date2,(end_m-start_m-1));
    %hold on, ax.XTick = xData;
    %hold on, datetick('x','MM','keepticks');
    hold on, lgd = legend('show');
    hold on, lgd.FontSize = 20;
    hold off;
end

if (0)
    data = load([path '/out_proximity_interval.txt']);
    %[h, m, s] = transferTime(data(:,1));
    %inds = (h-start_h)*3600 + (m - start_m) * 60 + ceil(s);
    inds = data(:,1) - data(1,1) + 1;
    nnd1 = data(:,3);
    nnd2 = data(:,5);
    nnd3 = data(:,7);
    nnd4 = data(:,9);
    nndm = data(:,15);
    avgm = data(:,17);
    all_num = [nnd1;nnd2;nnd3;nnd4;nndm;avgm];
    colors = 'ymcrgbwk';
    markers = 'o+*.xsd^';
    legend_txt1 = 'NND 1';
    legend_txt2 = 'NND 2';
    legend_txt3 = 'NND 3';
    legend_txt4 = 'NND 4';
    legend_txtm = 'NND Mean';
    legend_txta = 'Average Dis';
    figure,
    %hold on, plot(inds/60, nnd1, [colors(1)], 'MarkerSize', 10, 'DisplayName',legend_txt1);
    %hold on, plot(inds/60, nnd2, [colors(2)], 'MarkerSize', 10, 'DisplayName',legend_txt2);
    %hold on, plot(inds/60, nnd3, [colors(3)], 'MarkerSize', 10, 'DisplayName',legend_txt3);
    %hold on, plot(inds/60, nnd4, [colors(4)], 'MarkerSize', 10, 'DisplayName',legend_txt4);
    hold on, plot(inds/60, nndm, [colors(5)], 'LineWidth',3, 'DisplayName',legend_txtm);
    hold on, plot(inds/60, avgm, [colors(6)], 'LineWidth',3, 'DisplayName',legend_txta);
    
    %hold on, xlim([start_m end_m]);
    hold on, ylim([0 max(all_num)+0.01]);
    hold on, lgd = legend('show');
    hold on, lgd.FontSize = 20;
    hold off;
end

if (1)
    data = dlmread([path '/out_event.txt']);
    %[h, m, s] = transferTime(data(:,1));
    %inds = (h-start_h)*3600 + (m - start_m) * 60 + ceil(s);
    inds = data(:,1) - data(1,1) + 1;
    nnd1 = data(:,3);
    nnd2 = data(:,4);
    nnd3 = data(:,5);
    nnd4 = data(:,6);
    colors = 'ymcrgbwk';
    markers = 'o+*.xsd^';
    legend_txt1 = 'One Leaving Others';
    legend_txt2 = 'One left by Others';
    legend_txt3 = 'Concentrating';
    legend_txt4 = 'Spreading';
    figure,
    %hold on, plot(inds/60, nnd1, [colors(1)], 'MarkerSize', 10, 'DisplayName',legend_txt1);
    %hold on, plot(inds/60, nnd2, [colors(2)], 'MarkerSize', 10, 'DisplayName',legend_txt2);
    %hold on, plot(inds/60, nnd3, [colors(3)], 'MarkerSize', 10, 'DisplayName',legend_txt3);
    %hold on, plot(inds/60, nnd4, [colors(4)], 'MarkerSize', 10, 'DisplayName',legend_txt4);
    %hold on, plot(inds/60, nnd1, [colors(1)], 'LineWidth',3, 'DisplayName',legend_txt1);
    %hold on, plot(inds/60, nnd2, [colors(2)], 'LineWidth',3, 'DisplayName',legend_txt2);
    hold on, plot(inds/60, nnd3, [colors(3)], 'LineWidth',3, 'DisplayName',legend_txt3);
    hold on, plot(inds/60, nnd4, [colors(4)], 'LineWidth',3, 'DisplayName',legend_txt4);
    
    %hold on, xlim([start_m end_m]);
    hold on, ylim([0 2]);
    hold on, lgd = legend('show');
    hold on, lgd.FontSize = 20;
    hold off;
end